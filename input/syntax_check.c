/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:15:00 by saecker           #+#    #+#             */
/*   Updated: 2026/09/02 13:03:02 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	pipe_is_syntax_error(const char *line, size_t *i)
{
	int	op_len;

	op_len = (int)(*i + 1);
	while (is_blank(line[op_len]))
		op_len++;
	if (*i == skip_blanks(line, 0) || line[op_len] == '|'
		|| !line[op_len] || line[op_len] == '\n')
		return (1);
	(*i)++;
	return (0);
}

static int	redir_is_syntax_error(const char *line, size_t *i)
{
	int	op_len;

	op_len = redir_op_len(line, *i, 0);
	*i += (size_t)op_len;
	while (is_blank(line[*i]))
		(*i)++;
	if (!line[*i] || line[*i] == '\n' || line[*i] == '|'
		|| redir_op_len(line, *i, 0))
		return (1);
	return (0);
}

static void	toggle_quote(const char *line, size_t *i, char *quote)
{
	if (!*quote && (line[*i] == '\'' || line[*i] == '"'))
		*quote = line[(*i)++];
	else
	{
		*quote = 0;
		(*i)++;
	}
}

int	has_syntax_error(const char *line)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i] && line[i] != '\n')
	{
		if ((!quote && (line[i] == '\'' || line[i] == '"'))
			|| (quote && line[i] == quote))
			toggle_quote(line, &i, &quote);
		else if (!quote && line[i] == '|'
			&& pipe_is_syntax_error(line, &i))
			return (1);
		else if (!quote && redir_op_len(line, i, 0)
			&& redir_is_syntax_error(line, &i))
			return (1);
		else
			i++;
	}
	return (0);
}
