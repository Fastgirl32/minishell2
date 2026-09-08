/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_only.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 13:18:33 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Prints heredoc lines for a redirection-only command. */
static void	print_heredoc(t_command *heredoc)
{
	int	i;

	i = 0;
	while (heredoc->argv && heredoc->argv[i])
	{
		ft_putstr_fd(heredoc->argv[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

/*
0x241 means O_WRONLY | O_CREAT | O_TRUNC.
0x441 means O_WRONLY | O_CREAT | O_APPEND.
*/
/* Opens or prints one redirection-only operation. */
int	consume_redir_helper(struct s_redir *rd, t_command **heredoc, int *fd)
{
	if (is_heredoc_op(rd->av[rd->op_i]))
	{
		*heredoc = build_heredoc(rd->av[rd->op_i + 1]);
		if (!*heredoc)
			return (-1);
		print_heredoc(*heredoc);
		return (free_list(*heredoc), 0);
	}
	else if (!ft_strcmp(rd->av[rd->op_i] + 1, ">"))
		*fd = open(rd->av[rd->op_i + 1], 0x241, 0666);
	else if (!ft_strcmp(rd->av[rd->op_i] + 1, ">>"))
		*fd = open(rd->av[rd->op_i + 1], 0x441, 0666);
	else if (!ft_strcmp(rd->av[rd->op_i] + 1, "<"))
		*fd = open(rd->av[rd->op_i + 1], O_RDONLY);
	if (*fd < 0)
		return (perror("minishell:"), -1);
	ft_close(fd);
	return (0);
}

/* Executes a segment containing only redirections. */
int	consume_redir_only_segment(struct s_redir *rd)
{
	t_command	*heredoc;
	int			fd;

	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac && is_redirect_op(rd->av[rd->op_i]))
		rd->op_i += 2;
	if (rd->op_i != rd->ac)
		return (0);
	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac)
	{
		fd = -1;
		if (consume_redir_helper(rd, &heredoc, &fd) == -1)
			return (-1);
		rd->op_i += 2;
	}
	return (1);
}

/* Checks whether a pipe follows the current segment. */
int	has_pipe_after_segment(const char *line, size_t end)
{
	while (line[end] && is_blank(line[end]))
		end++;
	if (line[end] == '|')
		return (1);
	return (0);
}

/* Stores the operation that limits a command node. */
int	set_command_limiter(t_command *cmd, char *lim)
{
	free(cmd->limiter);
	if (lim && lim[0] == REDIR_MARKER)
		lim++;
	cmd->limiter = ft_strdup(lim);
	if (!cmd->limiter)
		return (0);
	return (1);
}
