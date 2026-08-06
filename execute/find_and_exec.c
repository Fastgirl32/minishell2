/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:11:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 15:59:51 by baal             ###   ########.fr       */
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
Searches for an executable in all PATH locations
Executes it
Exits with status 1 otherwise.
*/
t_status    find_and_exec(t_command *cmd, t_vars *vars)
{
    char    *path_str = get_var("PATH", vars);
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
    free_arr((void **)vars->env);
    close_all_not_std_fds(cmd);
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
