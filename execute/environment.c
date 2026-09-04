/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:34:55 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/04 12:53:26 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (!new_env)
    	return (NULL);
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
	if (!new_env)
    	return;
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
void	env_remove(t_vars *vars, char *str)
{
	int		i;
	int		j;
	char	**new_env;
	char	*tmp;

	i = 0;
	j = 0;
	while ((vars->env)[i])
		i++;
	new_env = malloc(sizeof(char **) * (i + 1));
	if (!new_env)
    	return;
	i = 0;
	while ((vars->env)[j])
	{
		tmp = key((vars->env)[j]);
		if (!ft_strcmp(tmp, str))
			j++;
		free(tmp);
		new_env[i++] = ft_strdup((vars->env)[j]);
		j++;
	}
	new_env[i] = NULL;
	free_arr((void **)vars->env);
	vars->env = new_env;
	return ;
}
