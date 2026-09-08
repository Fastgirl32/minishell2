/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_delimiter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 08:20:00 by saecker           #+#    #+#             */
/*   Updated: 2026/09/08 08:32:52 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Copies one raw delimiter character while removing quotes. */
static int	append_raw_char(struct s_raw_word *raw, const char *line,
		size_t *i, char *quote)
{
	if (!*quote && (line[*i] == '\'' || line[*i] == '"'))
		*quote = line[(*i)++];
	else if (*quote && line[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
	}
	else if (!append_char(raw->word, raw->len, raw->cap, line[(*i)++]))
		return (0);
	return (1);
}

/* Checks whether a raw delimiter word continues at the current position. */
static int	raw_word_continues(const char *line, size_t i, size_t end,
		char quote)
{
	if (quote)
		return (1);
	if (is_blank(line[i]) || line[i] == '|')
		return (0);
	return (!redir_op_len(line, i, 0) && i < end);
}

/* Reads a heredoc delimiter without expanding its contents. */
static char	*raw_heredoc_word(const char *line, size_t *i, size_t end)
{
	t_raw	raw;
	char	*word;
	char	quote;
	size_t	len;
	size_t	cap;

	while (*i < end && is_blank(line[*i]))
		(*i)++;
	word = malloc(32);
	if (!word)
		return (NULL);
	word[0] = '\0';
	len = 0;
	cap = 32;
	quote = 0;
	raw.word = &word;
	raw.len = &len;
	raw.cap = &cap;
	while (*i < end && raw_word_continues(line, *i, end, quote))
		if (!append_raw_char(&raw, line, i, &quote))
			return (free(word), NULL);
	return (word);
}

/* Replaces an expanded delimiter token with its raw form. */
static int	replace_delimiter(struct s_delimiter *ctx, int op_len)
{
	char	*word;

	ctx->j++;
	ctx->i += (size_t)op_len;
	word = raw_heredoc_word(ctx->line, &ctx->i, ctx->end);
	if (!word || !ctx->av[ctx->j])
		return (free(word), 0);
	free(ctx->av[ctx->j]);
	ctx->av[ctx->j++] = word;
	return (1);
}

/* Processes one token while preserving heredoc delimiters. */
int	process_delimiter(struct s_delimiter *ctx)
{
	int	op_len;

	while (ctx->i < ctx->end && is_blank(ctx->line[ctx->i]))
		ctx->i++;
	op_len = redir_op_len(ctx->line, ctx->i, 0);
	if (op_len && ctx->line[ctx->i] == '<' && op_len == 2)
		return (replace_delimiter(ctx, op_len));
	if (op_len)
	{
		ctx->i += (size_t)op_len;
		ctx->j++;
		ctx->i = next_word_end(ctx->line, ctx->i, ctx->end);
		ctx->j++;
	}
	else
	{
		ctx->i = next_word_end(ctx->line, ctx->i, ctx->end);
		ctx->j++;
	}
	return (1);
}
