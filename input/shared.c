/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:36 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 13:10:49 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Checks whether a character is shell whitespace. */
int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

/* Checks whether a string contains only whitespace. */
int	is_blank_line(const char *s)
{
	while (*s)
	{
		if (!is_blank(*s) && *s != '\n')
			return (0);
		s++;
	}
	return (1);
}

/* Checks whether a character may occur in a variable name. */
int	is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

/* Returns the length of a redirection operator at a position. */
int	redir_op_len(const char *line, size_t i, char quote)
{
	if (quote)
		return (0);
	if (line[i] == '<' || line[i] == '>')
	{
		if (line[i + 1] == line[i])
			return (2);
		if (line[i] == '>' && line[i + 1] == '|')
			return (2);
		return (1);
	}
	return (0);
}

/* Finds the end of one command segment. */
size_t	segment_end(const char *line, size_t start)
{
	size_t	i;
	char	quote;

	i = start;
	quote = 0;
	while (line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote && line[i] == '\n')
			break ;
		else if (!quote && redir_op_len(line, i, 0))
		{
			i += (size_t)redir_op_len(line, i, 0);
			continue ;
		}
		else if (!quote && line[i] == '|')
			break ;
		i++;
	}
	while (i > start && is_blank(line[i - 1]))
		i--;
	return (i);
}
