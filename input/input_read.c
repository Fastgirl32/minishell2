/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:07 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/09 18:59:22 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Reads one line with readline and updates history. */
char	*read_line_prompt(t_vars *vars, const char *prompt)
{
	char	*line;

	(void)vars;
	line = readline(prompt);
	return (line);
}

/* Reads one plain line from standard input. */
char	*read_line_plain(void)
{
	char	*line;
	size_t	len;
	size_t	cap;
	char	c;
	ssize_t	bytes;

	line = malloc(1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	len = 0;
	cap = 1;
	while (1)
	{
		bytes = read(STDIN_FILENO, &c, 1);
		if (bytes <= 0)
			break ;
		if (!append_char(&line, &len, &cap, c))
			return (free(line), NULL);
		if (c == '\n')
			break ;
	}
	if (bytes < 0 || (bytes == 0 && len == 0))
		return (free(line), NULL);
	return (line);
}

/* Chooses interactive or plain shell input. */
char	*read_shell_line(t_vars *vars, const char *prompt)
{
	if (isatty(STDIN_FILENO))
		return (read_line_prompt(vars, prompt));
	return (read_line_plain());
}

/* Joins a continued line to the previous input. */
char	*append_line(char *line, char *more)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(line, "\n");
	free(line);
	if (!tmp)
		return (free(more), NULL);
	joined = ft_strjoin(tmp, more);
	free(tmp);
	free(more);
	return (joined);
}

/* Returns the prompt used for an unfinished quote. */
const char	*quote_prompt(char q)
{
	if (q == '"')
		return ("dquote> ");
	return ("quote> ");
}
