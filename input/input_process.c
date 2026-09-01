/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:24:58 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:25:02 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/*
Returns what $PS1 expands to, or NULL otherwise.
The caller will then use the fallback.
*/
char	*get_prompt(t_vars *vars, char *backup)
{
	char	*prompt;
	char	*tmp;

	ft_memcpy(backup, "minishell> ", 12);
	prompt = get_var("PS1", vars);
	if (!prompt || !prompt[0])
		prompt = NULL;
	else
	{
		tmp = prompt;
		prompt = expand_str(tmp, vars);
		free(tmp);
	}
	if (!prompt || !prompt[0])
		prompt = NULL;
	return (prompt);
}

/*
Attempts to use $PS1 as prompt.
If PS1 is not set or a malloc fails, "minishell> " is used as fallback.
Then calls the other function that read, parse and execute a new command.
*/
void	input_process(t_vars *vars)
{
	char	*line;
	char	*prompt;
	char	prompt_backup[12];

	prompt = get_prompt(vars, prompt_backup);
	if (!prompt)
		prompt = prompt_backup;
	line = read_shell_line(vars, prompt);
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
	history_add(vars, line);
	make_list(vars, line);
	free(line);
}
