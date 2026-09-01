/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:23:43 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:23:50 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_expand(struct s_expand *ex, const char *line, char **env,
		char *dyn)
{
	ex->line = line;
	ex->env = env;
	ex->dyn = dyn;
	ex->len = 0;
	ex->cap = 32;
	ex->quote = 0;
}

char	*copy_token(struct s_split *sp, size_t start)
{
	struct s_expand	ex;
	char			*token;
	char			*dyn;

	dyn = alloc_token_buffer();
	if (!dyn)
		return (NULL);
	ex.i = start;
	ex.end = sp->i;
	setup_expand(&ex, sp->line, sp->env, dyn);
	while (ex.i < ex.end)
	{
		if (consume_quote_char(&ex))
			continue ;
		if (!append_token_piece(&ex, *(sp->status)))
		{
			free(dyn);
			return (NULL);
		}
	}
	token = ft_strdup(ex.dyn);
	free(ex.dyn);
	return (token);
}
