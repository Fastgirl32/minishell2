#include "minishell.h"

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

char	*copy_token(const char *line, size_t start, size_t end, char **env)
{
	struct s_expand	ex;
	char			*token;
	char			*dyn;

	dyn = alloc_token_buffer();
	if (!dyn)
		return (NULL);
	ex.i = start;
	ex.end = end;
	setup_expand(&ex, line, env, dyn);
	while (ex.i < ex.end)
	{
		if (consume_quote_char(&ex))
			continue ;
		if (!append_token_piece(&ex))
		{
			free(dyn);
			return (NULL);
		}
	}
	token = ft_strdup(ex.dyn);
	free(ex.dyn);
	return (token);
}
