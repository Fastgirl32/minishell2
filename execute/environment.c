
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
Helper function.
*/
t_status	expand_str_util(char *format, char *rstr, int i[3], t_vars *vars)
{
	char	*tmptmp;
	char	*var;

	i[0]++;
	while (format[i[0] + i[1]] && format[i[0] + i[1]] != ' ' && format[i[0] + i[1]] != '$')
		(i[1])++;
	if (i[1] == 0)
		return (1);
	tmptmp = ft_strndup(format + i[0], i[1]);
	if (!tmptmp)
		return (2);
	var = get_var(tmptmp, vars);
	free(tmptmp);
	if (var)
		rstr = ft_str_append(rstr, var);
	i[0] += (i[1] - 1);
	i[2] = 0;
	return (0);
}

/*
Expands a string once according to environment variables.
i[0] is the cursor in the format string.
i[i] is the cursor in the variable name in the format string.
i[2] is the cursor in the non-variable in the format string.
*/
char	*expand_str_call(char *format, t_vars *vars)
{
	char	*rstr;
	char	*tmp;
	int		i[3];

	rstr = ft_calloc(ft_strlen(format) + 1, 1);
	tmp = malloc(ft_strlen(format) + 1);
	ft_bzero(i, 3);
	while (format[i[0]])
	{
		i[1] = 0;
		if (format[i[0]] != '$')
			tmp[(i[2])++] = format[i[0]];
		else
		{
			if (expand_str_util(format, rstr, i, vars) == 2)
				return (NULL);
		}
		i[0]++;
	}
	tmp[i[2]] = 0;
	rstr = ft_str_append(rstr, tmp);
	return (free(tmp), rstr);
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