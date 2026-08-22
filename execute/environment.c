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

	i = 0;
	j = 0;
	while ((vars->env)[i])
		i++;
	new_env = malloc(sizeof(char **) * (i + 1));
	i = 0;
	while ((vars->env)[j])
	{
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
