/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:15:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 17:30:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	pipe_is_syntax_error(const char *line, size_t *i)
{
	size_t	next;

	next = *i + 1;
	while (is_blank(line[next]))
		next++;
	if (*i == skip_blanks(line, 0) || line[next] == '|' || !line[next]
		|| line[next] == '\n')
		return (1);
	(*i)++;
	return (0);
}

static int	redir_is_syntax_error(const char *line, size_t *i)
{
	size_t	op_len;
	char	quote;

	op_len = (size_t)redir_op_len(line, *i, 0);
	*i += op_len;
	while (is_blank(line[*i]))
		(*i)++;
	if (!line[*i] || line[*i] == '\n' || line[*i] == '|' || redir_op_len(line,
			*i, 0))
		return (1);
	if (line[*i] == '\'' || line[*i] == '"')
	{
		quote = line[(*i)++];
		while (line[*i] && line[*i] != quote)
			(*i)++;
		if (line[*i] == quote)
			(*i)++;
	}
	return (0);
}

int	has_syntax_error(const char *line)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i++];
		else if (quote && line[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (!quote && line[i] == '|' && pipe_is_syntax_error(line, &i))
			return (1);
		else if (!quote && redir_op_len(line, i, 0)
			&& redir_is_syntax_error(line, &i))
			return (1);
		else
			i++;
	}
	return (quote != 0);
}

int	line_is_delimiter(char *line, const char *delimiter)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (!ft_strcmp(line, delimiter));
}

char	*expand_heredoc_line(char *line, t_vars *vars)
{
	struct s_split	sp;

	sp.line = line;
	sp.end = ft_strlen(line);
	sp.i = sp.end;
	sp.env = vars->env;
	sp.status = &vars->status;
	return (copy_token(&sp, 0));
}
