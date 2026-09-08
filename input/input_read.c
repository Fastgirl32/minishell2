/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:07 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:33:37 by saecker          ###   ########.fr       */
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

	line = NULL;
	len = 0;
	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
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
