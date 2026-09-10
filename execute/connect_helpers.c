/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:07:13 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/10 01:43:56 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirect_limiter(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s,
				"<<") || !ft_strcmp(s, ">>") || !ft_strcmp(s, ">|")));
}

/*
Destroys, frees and reconnects the linked list from prev to cmd
*/
void	destroy_linked_list_segment(t_command **cmd, t_command **prev)
{
	t_command	*tmp;

	tmp = *cmd;
	*cmd = (*cmd)->next;
	tmp->next = NULL;
	free_list((*prev)->next);
	(*prev)->next = *cmd;
	*prev = *cmd;
}

/*
For a given command. dups all file descriptors
returns 0 on success
*/
t_status	redirect_all(t_command *cmd)
{
	setup_child_signals();
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, 0) == -1)
			return (ft_close(&cmd->fd_in), 1);
		ft_close(&cmd->fd_in);
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, 1) == -1)
			return (ft_close(&cmd->fd_out), 1);
		ft_close(&cmd->fd_out);
	}
	return (0);
}

/*
For pipeline command. dups all file descriptors
returns 0 on success
*/
t_status	dup_all(t_command *cmd)
{
	setup_child_signals();
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, 0) == -1)
			return (1);
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, 1) == -1)
			return (1);
	}
	return (0);
}
