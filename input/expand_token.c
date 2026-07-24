#include "minishell.h"

int	append_env_var(struct s_expand *ex)
{
	size_t	j;
	char	*var;
	char	*val;

	j = ex->i + 1;
	while (j < ex->end && is_var_char(ex->line[j]))
		j++;
	if (j == ex->i + 1)
		return (0);
	var = ft_substr(ex->line, (unsigned int)(ex->i + 1), j - (ex->i + 1));
	if (!var)
		return (-1);
	val = check_env(ex->env, var);
	if (val && !append_str(&ex->dyn, &ex->len, &ex->cap, val))
	{
		free(var);
		free(val);
		return (-1);
	}
	free(var);
	free(val);
	ex->i = j;
	return (1);
}

int	handle_dollar(struct s_expand *ex)
{
	int	res;

	if ((ex->i + 1) < ex->end && ex->line[ex->i + 1] == '?')
	{
		if (!append_status(&ex->dyn, &ex->len, &ex->cap))
			return (-1);
		ex->i += 2;
		return (1);
	}
	res = append_env_var(ex);
	if (res < 0)
		return (-1);
	if (res > 0)
		return (1);
	return (0);
}

int	consume_quote_char(struct s_expand *ex)
{
	if (!ex->quote && (ex->line[ex->i] == '\'' || ex->line[ex->i] == '"'))
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

int	append_token_piece(struct s_expand *ex)
{
	int	res;

	if (ex->line[ex->i] == '$' && ex->quote != '\'')
	{
		res = handle_dollar(ex);
		if (res < 0)
			return (0);
		if (res > 0)
			return (1);
	}
	if (!append_char(&ex->dyn, &ex->len, &ex->cap, ex->line[ex->i]))
		return (0);
	ex->i++;
	return (1);
}

char	*alloc_token_buffer(void)
{
	char	*dyn;

	dyn = malloc(32);
	if (!dyn)
		return (NULL);
	dyn[0] = '\0';
	return (dyn);
}
