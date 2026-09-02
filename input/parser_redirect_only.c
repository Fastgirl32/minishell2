/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_only.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/02 12:57:35 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	try_open_close(char *path, int flags, t_vars *vars)
{
	int	fd;

	fd = open(path, flags, 0666);
	if (fd < 0)
	{
		perror(path);
		*(vars->status) = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

static int	consume_one_redir(struct s_redir *rd)
{
	t_command	*heredoc;

	if (!ft_strcmp(rd->av[rd->op_i], "<<"))
	{
		heredoc = build_heredoc(rd->av[rd->op_i + 1]);
		if (!heredoc)
			return (-1);
		free_list(heredoc);
		return (0);
	}
	if (!ft_strcmp(rd->av[rd->op_i], ">"))
		return (try_open_close(rd->av[rd->op_i + 1],
				O_WRONLY | O_CREAT | O_TRUNC, rd->vars));
	if (!ft_strcmp(rd->av[rd->op_i], ">>"))
		return (try_open_close(rd->av[rd->op_i + 1],
				O_WRONLY | O_CREAT | O_APPEND, rd->vars));
	return (try_open_close(rd->av[rd->op_i + 1], O_RDONLY, rd->vars));
}

int	consume_redir_only_segment(struct s_redir *rd)
{
	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac && rd->is_op[rd->op_i]
		&& is_redirect_op(rd->av[rd->op_i]))
		rd->op_i += 2;
	if (rd->op_i != rd->ac)
		return (0);
	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac)
	{
		if (consume_one_redir(rd) < 0)
			return (-1);
		rd->op_i += 2;
	}
	return (1);
}

int	has_pipe_after_segment(const char *line, size_t end)
{
	while (line[end] && is_blank(line[end]))
		end++;
	if (line[end] == '|')
		return (1);
	return (0);
}
