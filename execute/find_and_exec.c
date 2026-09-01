/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:11:28 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/31 15:35:01 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Closes a fd, passed by reference,
if it is not standard and sets it to -1 to avoid double closing.
*/
void	ft_close(int *fd)
{
	if (*fd != 0 && *fd != 1 && *fd != 2 && *fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

/*
Truncates a path string after the ':' separator.
*/
char	*getpath(char *str)
{
	int		i;
	char	*path;

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
void	close_all_not_std_fds(t_command *head)
{
	while (head)
	{
		ft_close(&head->fd_in);
		ft_close(&head->fd_out);
		head = head->next;
	}
}

/*
if "cmd->command" starts with a '/', it is absolute,
therefore not changed and directly accessed. 
Else, "cmd->command" gets appended to "path_scan" and then accessed.
Path is set to whatever it actually is.
*/
int	access_path(char *path_scan, char path[][4096], t_command *cmd)
{
	char	*tmp;
	char	*tmp2;
	int		found;

	if (!path)
		return (-1);
	if (cmd->command[0] == '/')
	{
		found = access(cmd->command, F_OK);
		ft_memcpy(*path, cmd->command, ft_strlen(cmd->command) + 1);
		return (found);
	}
	tmp2 = getpath(path_scan + 1);
	tmp = ft_strjoin(tmp2, "/");
	free(tmp2);
	tmp2 = ft_strjoin(tmp, cmd->command);
	free(tmp);
	found = access(tmp2, F_OK);
	ft_memcpy(*path, tmp2, ft_strlen(tmp2) + 1);
	free(tmp2);
	return (found);
}

/*
Searches for an executable in all PATH locations
Executes it
Exits with status 1 otherwise.
*/
t_status	find_and_exec(t_command *cmd, t_vars *vars)
{
	char	*path_scan;
	char	*path_scan_dup;
	char	path[PATH_MAX];
	int		found;

	path_scan = get_var("PATH", vars);
	path_scan_dup = path_scan;
	found = -1;
	while (path_scan && found == -1)
	{
		found = access_path(path_scan, &path, cmd);
		path_scan = ft_strchr(path_scan + 1, ':');
	}
	free(path_scan_dup);
	free_vars(vars);
	close_all_not_std_fds(cmd);
	if (found == -1)
	{
		perror(cmd->command);
		exit(1);
	}
	else
		execv(path, cmd->argv);
	return (127);
}
