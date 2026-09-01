/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_only.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:25:28 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (!ft_strcmp(rd->av[rd->op_i], "<<"))
		{
			heredoc = build_heredoc(rd->av[rd->op_i + 1]);
			if (!heredoc)
				return (-1);
			free_list(heredoc);
		}
		else if (!ft_strcmp(rd->av[rd->op_i], ">"))
		{
			fd = open(rd->av[rd->op_i + 1], O_WRONLY | O_CREAT | O_TRUNC,
					0666);
			if (fd < 0)
				return (-1);
			close(fd);
		}
		else if (!ft_strcmp(rd->av[rd->op_i], ">>"))
		{
			fd = open(rd->av[rd->op_i + 1], O_WRONLY | O_CREAT | O_APPEND,
					0666);
			if (fd < 0)
				return (-1);
			close(fd);
		}
		else if (!ft_strcmp(rd->av[rd->op_i], "<"))
		{
			fd = open(rd->av[rd->op_i + 1], O_RDONLY);
			if (fd < 0)
				return (-1);
			close(fd);
		}
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
