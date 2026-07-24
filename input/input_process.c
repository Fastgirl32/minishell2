#include "minishell.h"
#include <errno.h>

char	find_unclosed_quote(const char *s)
{
	char	q;

	q = 0;
	while (*s)
	{
		if (!q && (*s == '\'' || *s == '"'))
			q = *s;
		else if (q && *s == q)
			q = 0;
		s++;
	}
	return (q);
}

char	*read_continued_lines(t_vars *vars, char *line)
{
	char	q;
	char	*more;

	q = find_unclosed_quote(line);
	while (q)
	{
		more = read_shell_line(vars, quote_prompt(q));
		if (!more)
		{
			if (errno != EINTR)
				vars->stop = 1;
			free(line);
			return (NULL);
		}
		line = append_line(line, more);
		if (!line)
			return (NULL);
		q = find_unclosed_quote(line);
	}
	return (line);
}

void	input_process(t_vars *vars)
{
	char	*line;

	line = read_shell_line(vars, "minishell> ");
	if (!line)
	{
		if (errno != EINTR)
			vars->stop = 1;
		return ;
	}
	line = read_continued_lines(vars, line);
	if (!line || vars->stop)
		return (free(line));
	history_add(vars, line);
	make_list(vars, line);
	free(line);
}
