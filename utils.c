/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:02:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/03 18:30:22 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <sys/types.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 && s2)
		return (s2[0]);
	if (!s2 && s1)
		return (s1[0]);
	if (!s1 && !s2)
		return (0);
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

char	*get_input(int fd)
{
	char	*input;
	char	*tmp;
	char	readbuf[BUFFER_SIZE + 1];
	int		br;
	int		i;

	input = malloc(BUFFER_SIZE + 1);
	br = read(fd, input, BUFFER_SIZE);
	input[br] = 0;
	while (br > 0)
	{
		tmp = malloc(ft_strlen(input) + 1);
		i = -1;
		while (input[++i])
			tmp[i] = input[i];
		tmp[i] = 0;
		br = read(fd, readbuf, BUFFER_SIZE);
		readbuf[br] = 0;
		free(input);
		input = ft_strjoin(tmp, readbuf);
		ft_bzero(readbuf, BUFFER_SIZE + 1);
		free(tmp);
	}
	return (input);
}
char	*key(const char *str)
{
	int		i;
	int		j;
	char	*key;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=' && str[i] != '\n')
		i++;
	key = malloc(i + 1);
	j = 0;
	if (!key)
		return (NULL);
	while (j < i)
	{
		key[j] = str[j];
		j++;
	}
	key[j] = 0;
	return (key);
}

char	*value(const char *str)
{
	int		i;
	int		j;
	int		k;
	char	*value;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=' && str[i] != '\n')
		i++;
	while (str[i + j])
		j++;
	k = j;
	value = malloc(j + 1);
	if (!value)
		return (NULL);
	j = 0;
	i++;
	while (j < k)
	{
		value[j] = str[i];
		i++;
		j++;
	}
	value[j - 1] = 0;
	return (value);
}

char	**recreate_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_arr(void **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}


t_status	contains_var(char **env, char *var)
{
	int		i;
	char	*tmp;
	char	*key1;
	char	*key2;

	i = 0;
	while (env[i])
	{
		key1 = key(env[i]);
		key2 = key(var);
		if (!ft_strcmp(key1, key2))
		{
			tmp = ft_strjoin(key(env[i]), "=");
			tmp = ft_strjoin(tmp, value(var));
			free(env[i]);
			env[i] = tmp;
			return (free(key1), free(key2), 1);
		}
		free(key1);
		free(key2);
		i++;
	}
	return (0);
}

void	env_append(char ***env, char *str)
{
	int			i;
	t_status	expand;
	char		**new_env;

	i = 0;
	while ((*env)[i])
		i++;
	expand = contains_var(*env, str);
	if (expand)
		return ;
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = ft_strdup(str);
	new_env[i + 1] = NULL;
	free_arr((void **)*env);
	*env = new_env;
	return ;
}

void		env_remove(char ***env, char *str)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc(sizeof(char **) * (i + 1));
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strcmp((*env)[i], str))
			continue ;
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = NULL;
	free_arr((void **)*env);
	*env = new_env;
}

void	print_var(int fd, char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(var, env[i], ft_strlen(var)))
		{
			ft_putstr_fd(value(env[i]), fd);
			return  ;
		}
		i++;
	}
	return ;
}

char*	get_var(char *var, char **env)
{
	int		i;
	char	*val;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(var, env[i], ft_strlen(var)))
		{
			val = value(env[i]);
			return (val);
		}
		i++;
	}
	return (NULL);
}

void	print_env_fd(int fd, char *format, char **env)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (format[i])
	{
		j = 0;
		if (format[i] != '$')
			write(1, format+i, 1);
		else
		{
			i++;
			while (format[i + j] && format[i + j] != ' ' && format[i + j] != '$')
				j++;
			if (j == 0)
				continue ;
			var = malloc(j + 1);
			ft_memcpy(var, format + i , j);
			var[j] = 0;
			print_var(fd, var, env);
			free(var);
			i += (j - 1);
		}
		i++;
	}
}

char	*expand_str(char *format, char **env)
{
	char	*rstr = ft_calloc(ft_strlen(format) + 1, 1);
	char	*tmp = malloc(ft_strlen(format) + 1);
	char	*tmptmp;
	char	*var;
	int		i[3] = {0};
	while (format[i[0]])
	{
		i[1] = 0;
		if (format[i[0]] != '$')
			tmp[(i[2])++] = format[i[0]];
		else
		{
			i[0]++;
			while (format[i[0] + i[1]] && format[i[0] + i[1]] != ' ' && format[i[0] + i[1]] != '$')
				(i[1])++;
			if (i[1] == 0)
				continue ;
			tmptmp = malloc(i[1] + 1);
			ft_memcpy(tmptmp, format + i[0] , i[1]);
			tmptmp[i[1]] = 0;
			var = get_var(tmptmp, env);
			free(tmptmp);
			tmp[i[2]] = 0;
			tmptmp = ft_strjoin(rstr, tmp);
			free(rstr);
			rstr = tmptmp;
			if (var)
			{
				tmptmp = ft_strjoin(rstr, var);
				free(var);
				free(rstr);
				rstr = tmptmp;
			}
			i[0] += (i[1] - 1);
			i[2] = 0;
		}
		i[0]++;
	}
	tmp[i[2]] = 0;
	tmptmp = ft_strjoin(rstr, tmp);
	free(rstr);
	free(tmp);
	rstr = tmptmp;
	return (rstr);
}
