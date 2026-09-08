/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_extract.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:31 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:34:28 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Checks whether a token is a marked redirection operator. */
int	is_redirect_op(const char *s)
{
	return (s && s[0] == REDIR_MARKER
		&& (!ft_strcmp(s + 1, "<") || !ft_strcmp(s + 1, ">")
			|| !ft_strcmp(s + 1, "<<") || !ft_strcmp(s + 1, ">>")
			|| !ft_strcmp(s + 1, ">|")));
}

	/* Checks whether a token is a marked heredoc operator. */
int	is_heredoc_op(const char *s)
{
	return (is_redirect_op(s) && !ft_strcmp(s + 1, "<<"));
}

	/* Stores the delimiter associated with a redirection token. */
void	set_redirect_limit(char **limit, char **av, int i, int ac)
{
	if (*limit)
		return ;
	if (is_heredoc_op(av[i]) && i + 1 < ac)
		*limit = ft_strdup(av[i + 1]);
	else
		*limit = ft_strdup(av[i] + 1);
}

	/* Removes one redirection and its target from the token array. */
int	skip_redirect_token(char **av, int i, int ac, char **limit)
{
	set_redirect_limit(limit, av, i, ac);
	free(av[i]);
	av[i] = NULL;
	if (i + 1 < ac)
		free(av[i + 1]);
	return (i + 2);
}

	/* Removes redirection pairs and leaves normal command arguments. */
void	extract_redirections(char **av, int *ac, char **limit,
		int *redirect_start)
{
	int	i;
	int	j;

	if (!av || !ac || *ac < 1)
		return ;
	if (redirect_start)
		*redirect_start = is_redirect_op(av[0]);
	i = 0;
	j = 0;
	while (i < *ac)
	{
		if (is_redirect_op(av[i]))
			i = skip_redirect_token(av, i, *ac, limit);
		else
			av[j++] = av[i++];
	}
	av[j] = NULL;
	*ac = j;
}
