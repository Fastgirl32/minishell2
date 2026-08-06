/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:11:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 16:16:00 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Truncates a path string after the ':' separator.
*/
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

/*
Iterates through the linked list and closes all non standard fds of the pipeline.
This assumes dup2 has set up everything correctly.
*/
void        close_all_not_std_fds(t_command *head)
{
    while (head)
    {
        ft_close(&head->fd_in);
        ft_close(&head->fd_out);
        head = head->next;
    }
}
/*
Truncates path_scan, attempts to access it and returns the return value of the access syscall.
*/
int         access_path(char *path_scan, char **path, t_command *cmd)
{
    char    *tmp;
    int     found;

    free(*path);
    *path = getpath(path_scan + 1);
    tmp = ft_strjoin(*path, "/");
    free(*path);
    *path = ft_strjoin(tmp, cmd->command);
    free(tmp);
    found = access(*path, F_OK);
    return (found);
}

/*
Searches for an executable in all PATH locations
Executes it
Exits with status 1 otherwise.
*/
t_status    find_and_exec(t_command *cmd, t_vars *vars)
{
    char    *path_scan;
    char    *path;
    int     found;

    path_scan = get_var("PATH", vars);
    path = NULL;
    found = -1;
    while (path_scan && found == -1)
    {
        found = access_path(path_scan, &path, cmd);
        path_scan = ft_strchr(path_scan + 1, ':');
    }
    free_arr((void **)vars->env);
    close_all_not_std_fds(cmd);
    printf("%s\n", path);
    if (found == -1)
    {
        perror(cmd->command);
        exit(1);
    }
    else
        execv(path, cmd->argv);    
    return (free(path), free(path_scan), 127);
}
