#include "minishell.h"

int	is_redirect_op(const char *s)
{
	return (s && (!ft_strcmp(s, "<") || !ft_strcmp(s, ">")
			|| !ft_strcmp(s, "<<") || !ft_strcmp(s, ">>")));
}

int	is_heredoc_op(const char *s)
{
	return (s && !ft_strcmp(s, "<<"));
}

void	set_redirect_limit(char **limit, char **av, int i, int ac)
{
	if (*limit)
		return ;
	if (is_heredoc_op(av[i]) && i + 1 < ac)
		*limit = ft_strdup(av[i + 1]);
	else
		*limit = ft_strdup(av[i]);
}

int	skip_redirect_token(char **av, int i, int ac, char **limit)
{
	set_redirect_limit(limit, av, i, ac);
	free(av[i]);
	av[i] = NULL;
	if (i + 1 < ac)
		free(av[i + 1]);
	return (i + 2);
}

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
