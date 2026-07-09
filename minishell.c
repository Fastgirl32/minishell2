/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/09 19:46:40 by baal             ###   ########.fr       */
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
		if (pipe(pipe_pair) != 0)
			return (1);
		cmd->fd_out = pipe_pair[1];
		cmd->next->fd_in = pipe_pair[0];
		cmd = cmd->next;
	}
	cmd->fd_out = 1;
	return (0);
}
/*
single command: direkt ausführen
Teil einer pipeline: forken, dann als child ausführen
das tut alle edge cases abklären (cd Documents | echo hi wechselt nicht wirklich z.B.)
*/
void	execute(t_command *cmd, char **env_src)
{
	pid_t	child_pid;
	char	**env;

	child_pid = 0;
	if (cmd->next)
		child_pid = fork();
	if (!child_pid)
	{
		setup_child_signals();
		dup2(cmd->fd_in, 0);
		dup2(cmd->fd_out, 1);
		env = recreate_env(env_src);
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
		else
			find_and_exec(cmd, env);
		exit(0);
		free_arr((void **)env);
	}
	else if (cmd->next)
	{
	 	close(cmd->fd_in);
	 	close(cmd->fd_out);
	 	execute(cmd->next, env_src);
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
	print_banner();
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
