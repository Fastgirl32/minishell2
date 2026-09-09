/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:26:10 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/09 19:12:29 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	next_word_end(const char *line, size_t i, size_t end)
{
	char	quote;

	quote = 0;
	while (i < end)
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote && (is_blank(line[i]) || redir_op_len(line, i, 0)))
			break ;
		i++;
	}
	return (i);
}

size_t	count_tokens(const char *line, size_t start, size_t end)
{
	size_t	i;
	size_t	count;

	i = start;
	count = 0;
	while (i < end)
	{
		while (i < end && is_blank(line[i]))
			i++;
		if (i >= end)
			break ;
		if (redir_op_len(line, i, 0))
		{
			count++;
			i += (size_t)redir_op_len(line, i, 0);
			continue ;
		}
		count++;
		i = next_word_end(line, i, end);
	}
	return (count);
}

void	free_tokens(char **av, size_t used)
{
	while (used > 0)
		free(av[--used]);
	free(av);
}
