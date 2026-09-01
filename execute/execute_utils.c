/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:02:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/07 14:58:06 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stddef.h>

/*
Compares two C-strings. Easier to use than strncmp.
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

/*
Duplicates a string up to n bytes.
*/
char	*ft_strndup(char *str, size_t n)
{
	char	*rstr;

	rstr = malloc(n + 1);
	if (!rstr)
		return (NULL);
	ft_memcpy(rstr, str, n);
	rstr[n] = 0;
	return (rstr);
}

/*
Appends s2 to s1 and frees s1.
Proper use:
s1 = ft_str_append(s1, s2);
Returns s1 on fail.
*/
char	*ft_str_append(char *s1, char *s2)
{
	char	*rstr;

	rstr = ft_strjoin(s1, s2);
	if (!rstr)
		return (s1);
	free(s1);
	return (rstr);
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
Returns 1 if a command is builtin.
0 otherwise
*/
t_u16	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "unset"));
}
