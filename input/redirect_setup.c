/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:53:14 by saecker           #+#    #+#             */
/*   Updated: 2026/09/02 12:55:34 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	report_redir_fail(char *target, t_command *prev)
{
	perror(target);
	prev->redir_failed = 1;
}

static void	open_write_redirect(t_command *cmd, t_command *prev, int flags)
{
	int	fd_tmp;

	if (prev->redir_failed)
		return ;
	fd_tmp = prev->fd_out;
	prev->fd_out = open(cmd->next->command, O_WRONLY | O_CREAT | flags, 0666);
	ft_close(&fd_tmp);
	if (prev->fd_out == -1)
		report_redir_fail(cmd->next->command, prev);
}

static void	open_read_redirect(t_command *cmd, t_command *prev)
{
	int	fd_tmp;

	if (prev->redir_failed)
		return ;
	fd_tmp = prev->fd_in;
	prev->fd_in = open(cmd->next->command, O_RDONLY);
	ft_close(&fd_tmp);
	if (prev->fd_in == -1)
		report_redir_fail(cmd->next->command, prev);
}

void	open_redirect_target(t_command *cmd, t_command *prev)
{
	if (!ft_strcmp(cmd->limiter, ">") || !ft_strcmp(cmd->limiter, ">|"))
		open_write_redirect(cmd, prev, O_TRUNC);
	else if (!ft_strcmp(cmd->limiter, "<"))
		open_read_redirect(cmd, prev);
	else if (!ft_strcmp(cmd->limiter, ">>"))
		open_write_redirect(cmd, prev, O_APPEND);
}

t_status	handle_heredoc_redirect(t_command *cmd, t_command *prev)
{
	int	pipe_fd[2];
	int	i;

	if (pipe(pipe_fd) != 0)
		return (1);
	i = 0;
	while (cmd->next->argv && cmd->next->argv[i])
	{
		write(pipe_fd[1], cmd->next->argv[i], ft_strlen(cmd->next->argv[i]));
		write(pipe_fd[1], "\n", 1);
		i++;
	}
	ft_close(&pipe_fd[1]);
	ft_close(&prev->fd_in);
	prev->fd_in = pipe_fd[0];
	return (0);
}
