/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:11:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/21 15:46:50 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char* getpath(char *str)
{
    int     i;
    char*   path;

    i = 0;
    path = malloc(ft_strlen(str) + 1);
    while (str[i] && str[i] != ':')
    {
        path[i] = str[i];
        i++;
    }
    path[i] = 0;
    return (path);
}

t_status    find_and_exec(t_command *cmd, char **env)
{
    char    *path_str = get_var("PATH", env);
    char    *path_scan = path_str;
    char    *path = NULL;
    char    *tmp;
    int     found = -1;
    while (path_scan && found == -1)
    {
        free(path);
        path = getpath(path_scan + 1);
        tmp = ft_strjoin(path, "/");
        free(path);
        path = ft_strjoin(tmp, cmd->command);
        free(tmp);
        found = access(path, F_OK);
        path_scan = ft_strchr(path_scan + 1, ':');
    }
    free_arr((void **)env);
    if (found == -1)
    {
        perror(cmd->command);
        exit(1);
    }
    else
        execv(path, cmd->argv);
    free(path_str);
    free(path);
    return (127);
}
