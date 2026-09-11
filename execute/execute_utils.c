/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:02:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/11 14:04:42 by saecker          ###   ########.fr       */
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

void	cmd_not_found(char *cmd)
{
	char	*str;
	char	*str2;
	char	*str3;

	str = ft_strndup("minishell: ", 11);
	str2 = ft_strndup(cmd, ft_strlen(cmd));
	str3 = ft_strjoin(str, str2);
	free(str);
	free(str2);
	str = ft_strndup(": command not found\n", 21);
	str2 = ft_strjoin(str3, str);
	free(str3);
	free(str);
	write(2, str2, ft_strlen(str2));
	free(str2);
}

/*
Returns 1 if a command is builtin.
0 otherwise
*/
t_u16	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "unset"));
}
