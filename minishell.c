/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/02 12:54:27 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_limiter(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s,
				"<<") || !ft_strcmp(s, ">>") || !ft_strcmp(s, ">|")));
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

/*
ENTRY POINT
*/
int	main(int ac, char **av, char **env)
{
	t_vars	*vars;
	int		status;

	(void)ac;
	(void)av;
	status = 0;
	vars = init_vars(env, &status);
	setup_parent_signals();
	while (vars->stop == 0)
	{
		input_process(vars);
	}
	free_vars(vars);
	return (status);
}
