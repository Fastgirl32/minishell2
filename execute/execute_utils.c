/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:02:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 18:27:30 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Compares two strings. Easier to use than strncmp.
*/
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 && s2)
		return (s2[0]);
	if (!s2 && s1)
		return (s1[0]);
	if (!s1 && !s2)
		return (0);
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

char	*get_input(int fd)
{
	char	*input;
	char	*tmp;
	char	readbuf[BUFFER_SIZE + 1];
	int		br;
	int		i;

	input = malloc(BUFFER_SIZE + 1);
	br = read(fd, input, BUFFER_SIZE);
	input[br] = 0;
	while (br > 0)
	{
		tmp = malloc(ft_strlen(input) + 1);
		i = -1;
		while (input[++i])
			tmp[i] = input[i];
		tmp[i] = 0;
		br = read(fd, readbuf, BUFFER_SIZE);
		readbuf[br] = 0;
		free(input);
		input = ft_strjoin(tmp, readbuf);
		ft_bzero(readbuf, BUFFER_SIZE + 1);
		free(tmp);
	}
	return (input);
}

/*
Frees a double pointer passed as (void **)
*/
void	free_arr(void **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
Returns 1 if a command is builtin.
0 otherwise
*/
t_u16	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "echo") ||
		!ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit") ||
		!ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "pwd") ||
		!ft_strcmp(cmd, "unset"));
}
/*
Closes a fd, passed by reference, if it is not standard and sets it to -1 to avoid double closing.
*/
void	ft_close(int *fd)
{
	if (*fd != 0 && *fd != 1 && *fd != 2 && *fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}
