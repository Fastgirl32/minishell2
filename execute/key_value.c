/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:35:24 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:35:26 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
Returns 1 if the environment in vars contains the variable var,
0 otherwise.
Replaces the value of the variable with the value of var, if they differ.
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
Returns the value of a key, found in the environment of vars.
NULL if it is not found.
*/
char	*get_var(char *var, t_vars *vars)
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
