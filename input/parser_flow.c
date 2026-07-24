#include "minishell.h"

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

void	parse_segment(struct s_redir *rd, const char *line, size_t start)
{
	struct s_split	sp;
	size_t			end;

	end = segment_end(line, start);
	sp.env = rd->vars->env;
	sp.ac = &rd->ac;
	rd->av = split_tokens(line, start, end, &sp);
	if (!rd->av)
		return ;
	rd->has_pipe = has_pipe_after_segment(line, end);
	if (handle_redirect_segment(rd) == 0)
		append_plain_segment(rd);
	if (rd->av)
		free_arr((void **)rd->av);
}

size_t	skip_blanks(const char *line, size_t i)
{
	while (line[i] && is_blank(line[i]))
		i++;
	return (i);
}

size_t	parse_and_move(struct s_redir *rd, const char *line, size_t i)
{
	i = skip_blanks(line, i);
	if (!line[i] || line[i] == '\n')
		return (i);
	parse_segment(rd, line, i);
	return (next_segment_start(line, segment_end(line, i)));
}

size_t	next_segment_start(const char *line, size_t pos)
{
	while (line[pos] && is_blank(line[pos]))
		pos++;
	if (line[pos] == '|')
		pos++;
	return (pos);
}
