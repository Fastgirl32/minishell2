/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:34:55 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/04 13:24:54 by saecker          ###   ########.fr       */
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
Appends a variable from the old env to the new env if it shall NOT be removed.
Otherwise skips it, effectively removing it.
Damn norminette.
*/
void	env_remove_helper(int i[2], t_vars *vars, char *str, char **new_env)
{
	char	*tmp;

	tmp = key((vars->env)[i[1]]);
	if (!ft_strcmp(tmp, str))
	{
		i[1]++;
		free(tmp);
		return ;
	}
	free(tmp);
	new_env[i[0]++] = ft_strdup((vars->env)[i[1]++]);
}

/*
Removes a variable string from the environment in vars.
*/
void	env_remove(t_vars *vars, char *str)
{
	int		i[2];
	char	**new_env;

	ft_bzero(i, sizeof(i));
	while ((vars->env)[i[0]])
		i[0]++;
	new_env = malloc(sizeof(char **) * (i[0] + 1));
	if (!new_env)
    	return;
	i[0] = 0;
	while ((vars->env)[i[1]])
		env_remove_helper(i, vars, str, new_env);
	new_env[i[0]] = NULL;
	free_arr((void **)vars->env);
	vars->env = new_env;
	return ;
}
