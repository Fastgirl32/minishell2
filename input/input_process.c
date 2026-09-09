/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:24:58 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/10 00:20:35 by saecker          ###   ########.fr       */
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
/* Builds the prompt from PS1 or the default fallback. */
char	*get_prompt(t_vars *vars, char *backup, _Bool *fallback_used)
{
	char	*prompt;
	char	*tmp;

	*fallback_used = 0;
	ft_memcpy(backup, "minishell> ", 12);
	prompt = get_var("PS1", vars);
	if (!prompt || !prompt[0])
	{
		prompt = NULL;
		*fallback_used = 1;
	}
	else
	{
		tmp = prompt;
		prompt = expand_str(tmp, vars);
		free(tmp);
	}
	if (!prompt || !prompt[0])
	{
		prompt = NULL;
		*fallback_used = 1;
	}
	return (prompt);
}

/* Stores, parses, and frees one complete input line. */
static void	process_line(t_vars *vars, char *line)
{
	size_t	len;

	vars->line = line;
	vars->history_entry = ft_strdup(line);
	if (vars->history_entry)
	{
		len = ft_strlen(vars->history_entry);
		if (len > 0 && vars->history_entry[len - 1] == '\n')
			vars->history_entry[len - 1] = '\0';
	}
	make_list(vars, line);
	if (vars->heredoc_interrupted)
	{
		*(vars->status) = 130;
		vars->heredoc_interrupted = 0;
		free(vars->history_entry);
		vars->history_entry = NULL;
		free(line);
		vars->line = NULL;
		return ;
	}
	setup_parent_signals();
	history_add(vars, vars->history_entry);
	free(vars->history_entry);
	vars->history_entry = NULL;
	free(line);
	vars->line = NULL;
}

/*
Attempts to use $PS1 as prompt.
If PS1 is not set or a malloc fails, "minishell> " is used as fallback.
Then calls the other function that read, parse and execute a new command.
*/
/* Reads and processes the next shell command line. */
void	input_process(t_vars *vars)
{
	char	*line;
	char	*prompt;
	char	prompt_fallback[12];
	_Bool	fallback_used;

	prompt = get_prompt(vars, prompt_fallback, &fallback_used);
	if (!prompt)
		prompt = prompt_fallback;
	line = read_shell_line(vars, prompt);
	if (!fallback_used)
		free(prompt);
	if (take_interactive_sigint())
		*(vars->status) = 130;
	if (!line)
	{
		if (errno != EINTR)
			vars->stop = 1;
		return ;
	}
	line = read_continued_lines(vars, line);
	if (!line || vars->stop)
		return (free(line));
	process_line(vars, line);
}
