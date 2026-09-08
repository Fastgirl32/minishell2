/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:23:43 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:32:52 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Consumes a quote and updates the current quote state. */
int	consume_quote_char(struct s_expand *ex)
{
	if (!ex->quote && (ex->line[ex->i] == '\''
			|| ex->line[ex->i] == '"'))
	{
		ex->quote = ex->line[ex->i];
		ex->i++;
		return (1);
	}
	if (ex->quote && ex->line[ex->i] == ex->quote)
	{
		ex->quote = 0;
		ex->i++;
		return (1);
	}
	return (0);
}

/* Copies one escaped character into the token buffer. */
int	append_escaped_piece(struct s_expand *ex)
{
	if ((ex->i + 1) < ex->end
		&& !append_char(&ex->dyn, &ex->len, &ex->cap, ex->line[ex->i + 1]))
		return (0);
	if ((ex->i + 1) < ex->end)
		ex->i += 2;
	else
		ex->i++;
	return (1);
}

/* Expands a dollar expression or leaves it literal. */
int	append_dollar_piece(struct s_expand *ex, int *status)
{
	int	res;

	res = handle_dollar(ex, status);
	if (res < 0)
		return (0);
	if (res > 0)
		return (1);
	if (!ex->quote && (ex->i + 1) < ex->end
		&& (ex->line[ex->i + 1] == '\''
			|| ex->line[ex->i + 1] == '"'))
	{
		ex->i++;
		return (1);
	}
	return (0);
}

/* Initializes the state used while copying a token. */
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

/* Copies and expands one token from the input line. */
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
