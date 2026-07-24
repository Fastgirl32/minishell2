#include "minishell.h"

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
	int		op_len;

	i = start;
	count = 0;
	while (i < end)
	{
		while (i < end && is_blank(line[i]))
			i++;
		if (i >= end)
			break ;
		op_len = redir_op_len(line, i, 0);
		if (op_len)
		{
			count++;
			i += (size_t)op_len;
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
