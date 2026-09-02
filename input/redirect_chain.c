/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_chain.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:53:14 by saecker           #+#    #+#             */
/*   Updated: 2026/09/02 13:06:18 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redir_node(t_command *node)
{
	ft_close(&node->fd_in);
	ft_close(&node->fd_out);
	free(node->command);
	free(node->limiter);
	free_arr((void **)node->argv);
	free(node);
}

static void	consume_redirect_node(t_command **cmd, t_command *prev,
		t_command **skip)
{
	*skip = *cmd;
	*cmd = (*cmd)->next;
	if (*skip != prev)
		free_redir_node(*skip);
}

static t_status	consume_redirect_chain(t_command **cmd, t_command *prev)
{
	t_command	*skip;

	while ((*cmd)->limiter && is_redirect_limiter((*cmd)->limiter))
	{
		if (!(*cmd)->next || !(*cmd)->next->command)
			break ;
		if (!ft_strcmp((*cmd)->limiter, "<<"))
		{
			if (handle_heredoc_redirect(*cmd, prev))
				return (1);
		}
		else
			open_redirect_target(*cmd, prev);
		consume_redirect_node(cmd, prev, &skip);
	}
	return (0);
}

static void	free_dangling_nodes(t_command *prev)
{
	t_command	*skip;

	while (prev->next)
	{
		skip = prev->next;
		prev->next = skip->next;
		free_redir_node(skip);
	}
}

t_u16	establish_redirects(t_command *top_cmd)
{
	t_command	*cmd;
	t_command	*prev;
	t_command	*skip;

	cmd = top_cmd;
	prev = cmd;
	while (cmd->next)
	{
		if (consume_redirect_chain(&cmd, prev))
			return (1);
		prev->next = cmd;
		if (cmd->limiter && !ft_strcmp(cmd->limiter, "|"))
		{
			consume_redirect_node(&cmd, prev, &skip);
			prev->next = cmd;
			prev = cmd;
		}
	}
	free_dangling_nodes(prev);
	return (0);
}
