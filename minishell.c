/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/25 21:24:16 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	is_redirect_limiter(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s,
				"<<") || !ft_strcmp(s, ">>")));
}

t_u16	connect_pipes(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*prev;
	int			pipe_pair[2];

	if (!top_cmd)
		return (0);
	cmd = top_cmd;
	prev = top_cmd;
	cmd->fd_in = 0;
	while (cmd->next)
	{
		while (cmd->limiter && is_redirect_limiter(cmd->limiter))
			cmd = cmd->next;
		if (cmd->limiter && !ft_strcmp(cmd->limiter, "|"))
		{
			cmd = cmd->next;
			if (pipe(pipe_pair) != 0)
					return (1);
			prev->fd_out = pipe_pair[1];
			cmd->fd_in = pipe_pair[0];
			prev = cmd;
		}
	}
	cmd->fd_out = 1;
	return (0);
}

t_u16	establish_redirects(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*prev;
	//int			fd;
	int			pipe_fd[2];
	int			i;

	cmd = top_cmd;
	prev = cmd;
	while (cmd->next)
	{
		while (cmd->limiter && is_redirect_limiter(cmd->limiter))
		{
			if (!cmd->next || !cmd->next->command)
				break ;
			if (!ft_strcmp(cmd->limiter, ">"))
			{
				prev->fd_out = open(cmd->next->command, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			}
			else if (!ft_strcmp(cmd->limiter, "<<"))
			{
				if (pipe(pipe_fd) != 0)
					return (1);
				i = 0;
				while (cmd->argv && cmd->argv[i])
				{
					write(pipe_fd[1], cmd->argv[i],
						ft_strlen(cmd->argv[i]));
					write(pipe_fd[1], "\n", 1);
					i++;
				}
				close(pipe_fd[1]);
				if (cmd->fd_in > 2)
					close(cmd->fd_in);
				cmd->fd_in = pipe_fd[0];
			}
			cmd = cmd->next;
		}
		if (cmd->limiter && !ft_strcmp(cmd->limiter, "|"))
		{
			cmd = cmd->next;
			prev->next = cmd;
			prev = cmd;
		}
	}
	prev->next = NULL;
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
	pid_t		child_pid;
	t_command	*next_cmd;
	char		**env;

	if (!cmd)
		return ;
	child_pid = fork();
	if (!child_pid) // in the child
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
		next_cmd = cmd->next;
		while (next_cmd && next_cmd->limiter && is_redirect_limiter(next_cmd->limiter) && next_cmd->next)
			next_cmd = next_cmd->next;
		if (next_cmd != cmd)
			execute(next_cmd, env_src);
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
	vars = init_vars(env);
	setup_parent_signals();
	// print_banner();
	while (vars->stop == 0)
	{
		input_process(vars);
	}
	return (0);
}
