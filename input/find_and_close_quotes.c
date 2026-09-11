/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_close_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 07:41:04 by saecker           #+#    #+#             */
/*   Updated: 2026/09/11 07:42:13 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Returns the quote character that is still open, if any. */
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

/* Reads additional lines until all quotes are closed. */
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
/*
Returns what $PS1 expands to, or NULL otherwise.
The caller will then use the fallback.
*/
