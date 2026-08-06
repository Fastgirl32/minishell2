/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:02:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 17:29:03 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Compares two strings. Easier to use than strncmp.
*/
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

/*
Returns a (char *) key from a (char *) like: "XXX=YYY" (key: "XXX")
Or the entire string if no '=' is found.
*/
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

/*
Returns a (char *) value from a (char *) like: "XXX=YYY" (value: "YYY")
Or NULL if no '=' is found.
*/
char	*value(const char *str)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	while (str[i] != '=')
	{
		if (!str[i] || str[i] == '\n')
			return (NULL);
		i++;
	}
	j = 0;
	while (str[i + j])
		j++;
	value = malloc(j + 1);
	if (!value)
		return (NULL);
	i++;
	ft_memcpy(value, str + i, j);
	value[j] = 0;
	return (value);
}

/*
Duplicates an environment on the heap.
*/
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

/*
Frees a double pointer passed as (void **)
*/
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

/*
Returns 1 if the environment in vars contains the variable var,
0 otherwise.
*/
t_status	contains_var(t_vars *vars, char *var)
{
	int		i;
	char	*tmp;
	char	*key1;
	char	*key2;

	i = 0;
	while (vars->env[i])
	{
		key1 = key(vars->env[i]);
		key2 = key(var);
		if (!ft_strcmp(key1, key2))
		{
			tmp = ft_strjoin(key(vars->env[i]), "=");
			tmp = ft_strjoin(tmp, value(var));
			free(vars->env[i]);
			vars->env[i] = tmp;
			return (free(key1), free(key2), 1);
		}
		free(key1);
		free(key2);
		i++;
	}
	return (0);
}

/*
Appends a variable string to the environment in vars.
*/
void	env_append(t_vars *vars, char *str)
{
	int			i;
	t_status	expand;
	char		**new_env;

	i = 0;
	while ((vars->env)[i])
		i++;
	expand = contains_var(vars, str);
	if (expand)
		return ;
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while ((vars->env)[i])
	{
		new_env[i] = ft_strdup((vars->env)[i]);
		i++;
	}
	new_env[i] = ft_strdup(str);
	new_env[i + 1] = NULL;
	free_arr((void **)vars->env);
	vars->env = new_env;
	return ;
}

/*
Removes a variable string from the environment in vars.
*/
void		env_remove(t_vars *vars, char *str)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while ((vars->env)[i])
		i++;
	new_env = malloc(sizeof(char **) * (i + 1));
	i = 0;
	while ((vars->env)[j])
	{
		//printf("i whole heartedly believe %s != %s\n", key((vars->env)[j]), str);
		if (!ft_strcmp(key((vars->env)[j]), str))
			j++;
		new_env[i] = ft_strdup((vars->env)[j]);
		j++;
		i++;
	}
	new_env[i] = NULL;
	free_arr((void **)vars->env);
	vars->env = new_env;
	return ;
}

/*
Returns the value of a key, found in the environment of vars.
NULL if it is not found.
*/
char*	get_var(char *var, t_vars *vars)
{
	int		i;
	char	*val;

	i = 0;
	while (vars->env[i])
	{
		if (!ft_strncmp(var, vars->env[i], ft_strlen(var)))
		{
			val = value(vars->env[i]);
			return (val);
		}
		i++;
	}
	return (NULL);
}

/*
Expands a string once according to environment variables.
*/
char	*expand_str_call(char *format, t_vars *vars)
{
	char	*rstr = ft_calloc(ft_strlen(format) + 1, 1);
	char	*tmp = malloc(ft_strlen(format) + 1);
	char	*tmptmp;
	char	*var;
	int		i[3];

	ft_bzero(i, 3);
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
			var = get_var(tmptmp, vars);
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

/*
Recursively expands a string according to environment variables.
*/
char	*expand_str(char *format, t_vars *vars)
{
	char	*rstr;
	char	*tmp;

	rstr = expand_str_call(format, vars);
	while (ft_strchr(rstr, '$'))
	{
		tmp = rstr;
		rstr = expand_str_call(tmp, vars);
		free(tmp);
	}
	tmp = rstr;
	rstr = ft_strtrim(tmp, " ");
	free(tmp);
	return (rstr);
}
/*
Returns 1 if a command is builtin.
0 otherwise
*/
t_u16	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "echo") ||
		!ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit") ||
		!ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "pwd") ||
		!ft_strcmp(cmd, "unset"));
}
/*
Closes a fd, passed by reference, if it is not standard and sets it to -1 to avoid double closing.
*/
void	ft_close(int *fd)
{
	if (*fd != 0 && *fd != 1 && *fd != 2 && *fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}
