/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:12 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:33:37 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Restores literal heredoc delimiters after token expansion. */
void	preserve_heredoc_delimiters(char **av, const char *line,
		size_t start, size_t end)
{
	struct s_delimiter	ctx;

	ctx.av = av;
	ctx.line = line;
	ctx.i = start;
	ctx.j = 0;
	ctx.end = end;
	while (ctx.i < ctx.end && ctx.av[ctx.j])
		if (!process_delimiter(&ctx))
			return ;
}

/* Builds a command from a segment without redirections. */
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

/* Parses one pipeline segment and appends its commands. */
void	parse_segment(struct s_redir *rd, const char *line,
	size_t start, int *status)
{
	struct s_split	sp;
	size_t			end;

	end = segment_end(line, start);
	sp.env = rd->vars->env;
	sp.ac = &rd->ac;
	sp.status = &status;
	rd->av = split_tokens(line, start, end, &sp);
	if (!rd->av)
		return ;
	preserve_heredoc_delimiters(rd->av, line, start, end);
	rd->has_pipe = has_pipe_after_segment(line, end);
	if (handle_redirect_segment(rd) == 0)
		append_plain_segment(rd);
	if (rd->av)
		free_arr((void **)rd->av);
}

/* Parses the current segment and returns the next input position. */
size_t	parse_and_move(struct s_redir *rd, const char *line,
	size_t i, int *status)
{
	i = skip_blanks(line, i);
	if (!line[i] || line[i] == '\n')
		return (i);
	parse_segment(rd, line, i, status);
	return (next_segment_start(line, segment_end(line, i)));
}
