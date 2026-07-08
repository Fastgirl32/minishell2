/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:08 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:10 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
extracts the value of the home variable or NULL
*/
char	*home_var(char **env)
{
	int		i;
	char	*key_str;
	char	*value_str;

	i = 0;
	while (env[i])
	{
		key_str = key(env[i]);
		if (!ft_strcmp("HOME", key_str))
		{
			value_str = value(env[i]);
			free(key_str);
			if (!value_str || !(value_str[0]))
				return (free(value_str), NULL);
			return (value_str);
		}
		free(key_str);
		i++;
	}
	return (NULL);
}
/*
possible cases:
	-absolute path given
	-relative path given
	-no path given & HOME is set
	-no path given & HOME is unset
*/
t_status	ft_cd(t_command *cmd, char **env)
{
	char	*curpath;
	char	*home;
	int		i;
	char	path[PATH_MAX];

	home = home_var(env);
	getcwd(path, PATH_MAX);
	i = ft_strlen(path) - 1;
	if (path[i] != '/')
	{
		path[i + 1] = '/';
		path[i + 2] = 0;
	}
	if (!home && cmd->argv[1] == NULL)
		return ((void)printf("cd: HOME not set\n"), 1);
	else if (home && cmd->argv[1] == NULL)
		curpath = ft_strdup(home);
	else if (cmd->argv[1] && cmd->argv[1][0] == '/')
		curpath = ft_strdup(cmd->argv[1]);
	else
		curpath = ft_strjoin(path, cmd->argv[1]);
	if (chdir(curpath))
	{
		if (cmd->argv[1] == NULL)
			printf("cd: no such file or directory: %s\n", curpath);
		else
			printf("cd: no such file or directory: <%s>\n", curpath);
		return (free(curpath), free(home), 1);
	}
	return (free(curpath), free(home), 0);
}

