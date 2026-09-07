/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 22:34:57 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/03 22:34:59 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redirect_limiter(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s,
				"<<") || !ft_strcmp(s, ">>") || !ft_strcmp(s, ">|")));
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
0x441 means O_WRONLY | O_CREAT | O_APPEND.
*/
t_u16	establish_redirect_helper2(t_command *cmd, t_command *prev,
	int pipe_fd[2], int *i)
{
	int	fd_tmp;

	if (!ft_strcmp(cmd->limiter, ">>"))
	{
		fd_tmp = prev->fd_out;
		prev->fd_out = open(cmd->next->command, 0x441, 0666);
		ft_close(&fd_tmp);
	}
	else if (!ft_strcmp(cmd->limiter, "<<"))
	{
		if (pipe(pipe_fd) != 0)
			return (1);
		*i = 0;
		while (cmd->next->argv && cmd->next->argv[*i])
		{
			write(pipe_fd[1], cmd->next->argv[*i],
				ft_strlen(cmd->next->argv[*i]));
			write(pipe_fd[1], "\n", 1);
			(*i)++;
		}
		ft_close(&pipe_fd[1]);
		ft_close(&prev->fd_in);
		prev->fd_in = pipe_fd[0];
	}
	return (0);
}

t_u16	establish_redirect_helper(t_command *cmd, t_command *prev,
	int pipe_fd[2], int *i)
{
	int			fd_tmp;

	if (!cmd->next || !cmd->next->command)
		return (0);
	if (!ft_strcmp(cmd->limiter, ">")
		|| !ft_strcmp(cmd->limiter, ">|"))
	{
		fd_tmp = prev->fd_out;
		prev->fd_out = open(cmd->next->command,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
		ft_close(&fd_tmp);
	}
	else if (!ft_strcmp(cmd->limiter, "<"))
	{
		fd_tmp = prev->fd_in;
		prev->fd_in = open(cmd->next->command, O_RDONLY);
		if (prev->fd_in < 0)
			perror("minishell");
		ft_close(&fd_tmp);
	}
	return (establish_redirect_helper2(cmd, prev, pipe_fd, i));
}

/*
Assumes pipes have been connected.
Establishes redirections in the linked list by
opening the right file descriptors and removing
elements with the > separator, leaving only a (possibly broken) pipeline.
*/
t_u16	establish_redirects(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*prev;
	int			pipe_fd[2];
	int			i;

	cmd = top_cmd;
	prev = cmd;
	while (cmd->next)
	{
		while (cmd->limiter && is_redirect_limiter(cmd->limiter))
		{
			if (establish_redirect_helper(cmd, prev, pipe_fd, &i))
				return (1);
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
