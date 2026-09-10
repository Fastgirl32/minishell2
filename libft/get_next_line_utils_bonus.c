/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:42:48 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/20 12:58:52 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	clear(char *stash)
{
	size_t	i;

	i = 0;
	while (i <= BUFFER_SIZE)
	{
		((unsigned char *)stash)[i] = 0;
		i++;
	}
}

size_t	safe_strlen(char const *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != 0)
		i++;
	return (i);
}

char	*join_and_free(char *s1, char *s2)
{
	char		*rstr;
	size_t		leng;
	size_t		i;
	size_t		pos[2];

	pos[0] = 0;
	pos[1] = 0;
	i = 0;
	leng = safe_strlen(s1) + safe_strlen(s2);
	rstr = malloc(leng + 1);
	if (rstr == NULL)
		return (free(s1), NULL);
	while (s1 && s1[pos[0]] != 0)
		rstr[i++] = s1[pos[0]++];
	while (s2 && s2[pos[1]] != 0)
		rstr[i++] = s2[pos[1]++];
	rstr[i] = 0;
	if (s2 && s1)
		free(s1);
	return (rstr);
}
