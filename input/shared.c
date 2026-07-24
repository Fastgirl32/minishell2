#include "minishell.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

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

int	is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	redir_op_len(const char *line, size_t i, char quote)
{
	if (quote)
		return (0);
	if (line[i] == '<' || line[i] == '>')
	{
		if (line[i + 1] == line[i])
			return (2);
		return (1);
	}
	return (0);
}

size_t	segment_end(const char *line, size_t start)
{
	size_t	i;
	char	quote;

	i = start;
	quote = 0;
	while (line[i] && line[i] != '\n')
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote && line[i] == '|')
			break ;
		i++;
	}
	while (i > start && is_blank(line[i - 1]))
		i--;
	return (i);
}
