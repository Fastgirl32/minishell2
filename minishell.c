/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 18:08:31 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirect_limiter(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s,
				"<<") || !ft_strcmp(s, ">>")));
}

/*
Recursively connects all the pipes in the linked list.
Skips redirections.
*/
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

/*
MUSS GEFIXT WERDEN (HEREDOC)
Assumes pipes have been connected.
Establishes redirections in the linked list by
opening the right file descriptors and removing
elements with the > separator, leaving only a (possibly broken) pipeline.
*/
t_u16	establish_redirects(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*prev;
	int			fd_tmp;
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
				fd_tmp = prev->fd_out;
				prev->fd_out = open(cmd->next->command, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				ft_close(&fd_tmp);
			}
			if (!ft_strcmp(cmd->limiter, ">>"))
			{
				fd_tmp = prev->fd_out;
				prev->fd_out = open(cmd->next->command, O_WRONLY | O_CREAT | O_APPEND, 0666);
				ft_close(&fd_tmp);
			}
			else if (!ft_strcmp(cmd->limiter, "<<"))
			{
				if (pipe(pipe_fd) != 0)
					return (1);
				i = 1;
				while (cmd->argv && cmd->argv[i])
				{
					write(pipe_fd[1], cmd->argv[i],
						ft_strlen(cmd->argv[i]));
					write(pipe_fd[1], "\n", 1);
					i++;
				}
				ft_close(&pipe_fd[1]);
				ft_close(&cmd->fd_in);
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
ENTRY POINT
*/
int	main(int ac, char **av, char **env)
{
	t_vars	*vars;

	(void)ac;
	(void)av;
	vars = init_vars(env);
	setup_parent_signals();
	print_banner();
	while (vars->stop == 0)
	{
		input_process(vars);
	}
	free_arr((void **)(vars->env));
	return (0);
}
