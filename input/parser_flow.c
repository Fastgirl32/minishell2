/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:12 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/09 19:00:15 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_pipe_after_segment(const char *line, size_t end)
{
	while (line[end] && is_blank(line[end]))
		end++;
	return (line[end] == '|');
}

void	append_plain_segment(struct s_redir *rd)
{
	t_command	*cmd;

	cmd = new_command(rd->av, rd->ac, rd->has_pipe);
	if (cmd)
		append_command(rd->head, rd->tail, cmd);
	else
		free_arr((void **)rd->av);
	rd->av = NULL;
}

void	parse_segment(struct s_redir *rd, const char *line, size_t start,
		int *status)
{
	struct s_split	sp;
	size_t			end;
	int				i;

	end = segment_end(line, start);
	sp.env = rd->vars->env;
	sp.ac = &rd->ac;
	sp.status = &status;
	rd->av = split_tokens(line, start, end, &sp);
	if (!rd->av)
		return ;
	rd->has_pipe = has_pipe_after_segment(line, end);
	i = 0;
	while (i < rd->ac && !is_redirect_token(rd->av[i]))
		i++;
	if (i < rd->ac)
		append_redirect_segment(rd);
	else
		append_plain_segment(rd);
	if (rd->av)
		free_arr((void **)rd->av);
}

size_t	parse_and_move(struct s_redir *rd, const char *line, size_t i,
		int *status)
{
	i = skip_blanks(line, i);
	if (!line[i] || line[i] == '\n')
		return (i);
	parse_segment(rd, line, i, status);
	return (next_segment_start(line, segment_end(line, i)));
}
