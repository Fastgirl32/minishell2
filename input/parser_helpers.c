/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:35:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 17:30:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	skip_blanks(const char *line, size_t i)
{
	while (line[i] && is_blank(line[i]))
		i++;
	return (i);
}

size_t	next_segment_start(const char *line, size_t pos)
{
	while (line[pos] && is_blank(line[pos]))
		pos++;
	if (line[pos] == '|')
		pos++;
	return (pos);
}

int	is_redirect_token(const char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, ">|")
		|| !ft_strcmp(token, "<<"));
}

int	is_heredoc_token(const char *token)
{
	return (!ft_strcmp(token, "<<"));
}

t_command	*target_command(char *target)
{
	char	**av;

	if (!target)
		return (NULL);
	av = malloc(sizeof(char *) * 2);
	if (!av)
		return (NULL);
	av[0] = ft_strdup(target);
	av[1] = NULL;
	if (!av[0])
		return (free(av), NULL);
	return (new_command(av, 1, 0));
}
