#include "minishell.h"

int	push_operator_token(struct s_split *sp)
{
	int		op_len;
	char	*token;

	op_len = redir_op_len(sp->line, sp->i, 0);
	if (!op_len)
		return (0);
	token = ft_substr(sp->line, (unsigned int)(sp->i), (size_t)op_len);
	if (!token)
		return (-1);
	sp->av[(sp->j)++] = token;
	sp->i += (size_t)op_len;
	return (1);
}

int	push_word_token(struct s_split *sp)
{
	size_t	start;
	char	*token;

	start = sp->i;
	sp->i = next_word_end(sp->line, sp->i, sp->end);
	token = copy_token(sp->line, start, sp->i, sp->env);
	if (!token)
		return (0);
	sp->av[(sp->j)++] = token;
	return (1);
}

int	fill_split_tokens(struct s_split *sp)
{
	int	res;

	while (sp->i < sp->end)
	{
		while (sp->i < sp->end && is_blank(sp->line[sp->i]))
			sp->i++;
		if (sp->i >= sp->end)
			break ;
		res = push_operator_token(sp);
		if (res < 0)
			return (0);
		if (res == 0 && !push_word_token(sp))
			return (0);
	}
	return (1);
}

char	**split_tokens(const char *line, size_t start, size_t end,
		struct s_split *sp)
{
	sp->av = malloc(sizeof(char *) * (count_tokens(line, start, end) + 1));
	if (!sp->av)
		return (NULL);
	sp->line = line;
	sp->end = end;
	sp->i = start;
	sp->j = 0;
	if (!fill_split_tokens(sp))
		return (free_tokens(sp->av, sp->j), NULL);
	sp->av[sp->j] = NULL;
	if (sp->ac)
		*sp->ac = (int)sp->j;
	return (sp->av);
}
