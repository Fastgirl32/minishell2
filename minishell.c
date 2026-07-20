/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/14 13:09:24 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_u16	connect_pipes(t_command *top_cmd)
{
	t_command	*cmd;
	int			pipe_pair[2];

	cmd = top_cmd;
	cmd->fd_in = 0;
	while (cmd->next)
	{
		if (!ft_strcmp(cmd->limiter, "|"))
		{
			if (pipe(pipe_pair) != 0)
				return (1);
			cmd->fd_out = pipe_pair[1];
			cmd->next->fd_in = pipe_pair[0];
		}
		cmd = cmd->next;
	}
	cmd->fd_out = 1;
	return (0);
}

t_u16	establish_redirects(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*next_cmd;
	int			fd;

	cmd = top_cmd;
	next_cmd = cmd;
	//printf("<%s>, <%s>\n", cmd->next->command, cmd->next->limiter);
	while (next_cmd && !ft_strcmp(next_cmd->limiter, ">"))
	{
		next_cmd = next_cmd->next;
		fd = open(next_cmd->command, O_WRONLY | O_CREAT | O_EXCL, 0666);
		cmd->fd_out = fd;
	}
	return (0);
}
/*
single command: direkt ausführen
Teil einer pipeline: forken, dann als child ausführen
das tut alle edge cases abklären (cd Documents | echo hi wechselt nicht wirklich z.B.)
*/

void	execute_builtin(t_command *cmd, char **env)
{
	if (!ft_strcmp(cmd->command, "echo"))
		ft_echo(cmd, env);
	else if (!ft_strcmp(cmd->command, "cd"))
		ft_cd(cmd, env);
	else if (!ft_strcmp(cmd->command, "pwd"))
		ft_pwd(cmd);
	else if (!ft_strcmp(cmd->command, "export"))
		ft_export(cmd, &env);
	else if (!ft_strcmp(cmd->command, "unset"))
		ft_unset(cmd, &env);
	else if (!ft_strcmp(cmd->command, "env"))
		ft_env(cmd, env);
	else if (!ft_strcmp(cmd->command, "exit"))
		ft_exit(cmd);
}

void	execute(t_command *cmd, char **env_src)
{
	pid_t	child_pid;
	char	**env;

	if (!cmd)
		return ;
	child_pid = fork();
	if (!child_pid) //in the child
	{
		setup_child_signals();
		if (cmd->fd_in != 0)
		{
			dup2(cmd->fd_in, 0);
			close(cmd->fd_in);
			cmd->fd_in = -1;
		}
		if (cmd->fd_out != 1)
		{
			dup2(cmd->fd_out, 1);
			close(cmd->fd_out);
			cmd->fd_out = -1;
		}
		env = recreate_env(env_src);
		if (is_builtin(cmd->command))
		{
			execute_builtin(cmd, env);
			free_arr((void **)env);
		}
		else
			find_and_exec(cmd, env);
		exit(0);
	}
	else
	{
		if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		execute(cmd->next, env_src);
		waitpid(child_pid, NULL, 0);
	}	
}

/*
DU WIRST DEN MAIN ENTRY POINT MACHEN
input parsing und so
*/
int	main(int ac, char **av, char **env)
{
	t_vars	*vars;

	(void)ac;
	(void)av;
	(void)env;
	vars = init_vars(env);
	setup_parent_signals();
	//print_banner();
	while (vars->stop == 0)
	{
		get_line(vars);
	}
	return (0);
	// hier simuliere ich input (hier gehört der Teil hin wo du input parsed)
	// t_command exit4 = {"invalid-4", 0, NULL, NULL, 0, 1};
	// t_command exit3 = {"invalid-3", 0, NULL, &exit4, 0, 1};
	// char *argv2[2] = {"rev", NULL};
	// t_command exit3 = {"rev", 1, argv2, NULL, 0, 1};
	// t_command exit2 = {"rev", 1, argv2, &exit3, 0, 1};
	// char *argv[3] = {"echo", "Der $PATH NAAAAAAAAHT\nnaa\nhd\nhdjhsdj",
	// 	NULL
	// };
	// t_command exit = {"echo", 2, argv, &exit2, 0, 1};
	// t_command pwd = {"echo", 1, NULL, NULL, 0, 1};
	// mein Teil
	// if (connect_pipes(vars->list))
	// 	return (1);
	// execute(vars->list, env);
}
