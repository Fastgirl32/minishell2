#include "minishell.h"

static int	grow_str(char **str, size_t *cap, size_t len)
{
	char	*new_str;
	size_t	new_cap;
	size_t	i;

	new_cap = *cap;
	if (new_cap == 0)
		new_cap = 32;
	while (len + 2 > new_cap)
		new_cap *= 2;
	new_str = malloc(new_cap);
	if (!new_str)
		return (0);
	i = 0;
	while (i < len)
	{
		new_str[i] = (*str)[i];
		i++;
	}
	new_str[len] = '\0';
	free(*str);
	*str = new_str;
	*cap = new_cap;
	return (1);
}

int	append_char(char **str, size_t *len, size_t *cap, char c)
{
	if (*len + 2 > *cap && !grow_str(str, cap, *len))
		return (0);
	(*str)[(*len)++] = c;
	(*str)[*len] = '\0';
	return (1);
}

int	append_str(char **str, size_t *len, size_t *cap, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		if (!append_char(str, len, cap, src[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*check_env(char **env, const char *name)
{
	int		i;
	size_t	name_len;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(name, env[i], name_len) && env[i][name_len] == '=')
			return (value(env[i]));
		i++;
	}
	return (NULL);
}

int	append_status(char **dyn, size_t *len, size_t *cap)
{
	char	*status_str;

	status_str = ft_itoa(g_status);
	if (!status_str)
		return (0);
	if (!append_str(dyn, len, cap, status_str))
	{
		free(status_str);
		return (0);
	}
	free(status_str);
	return (1);
}
