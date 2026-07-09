/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:36:20 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:47:44 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "libft.h"

static void	forget_all(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
		ptr[i] = NULL;
	}
	free(ptr);
	ptr = NULL;
}

static int	len_until(char const *str, char del)
{
	int	i;

	i = 0;
	while (str[i] != 0 && str[i] != del)
	{
		i++;
	}
	return (i);
}

static void	advance(char **str, char const *s, char c, int *pos)
{
	if (str[pos[0]] != NULL)
		str[pos[0]][pos[1]] = 0;
	if (pos[1] > 0)
		pos[0]++;
	if ((*s != c) && (*s != 0))
		str[pos[0]] = ft_calloc(len_until(s, c) + 1, 1);
	if (*s == 0)
	{
		free(str[pos[0]]);
		str[pos[0]] = NULL;
	}
	pos[1] = 0;
}

static int	count_words(char const *s, char c)
{
	int	wc;
	int	expect_word;

	expect_word = 1;
	wc = 0;
	while (*s != 0)
	{
		if (*s == c)
			expect_word = 1;
		else if (expect_word)
		{
			expect_word = 0;
			wc++;
		}
		s++;
	}
	return (wc + 1);
}

char	**ft_split(char const *s, char c)
{
	char	**rstr;
	int		*pos;

	pos = ft_calloc(2, sizeof(int));
	rstr = ft_calloc(count_words(s, c), sizeof(char *));
	if (rstr == NULL || pos == NULL)
		return (free(pos), free(rstr), NULL);
	if (!s || *s == '\0')
		return (free(pos), rstr[0] = NULL, rstr);
	if ((*s != c) && (*s != 0))
		rstr[pos[0]] = ft_calloc(len_until(s, c) + 1, 1);
	while (*s != 0)
	{
		while (*s == c)
			advance(rstr, ++s, c, pos);
		if (*s == 0)
			break ;
		if (rstr[pos[0]] != NULL)
			rstr[pos[0]][pos[1]++] = *s;
		else
			return (free(pos), forget_all(rstr), NULL);
		s++;
	}
	return (advance(rstr, s, c, pos), free(rstr[pos[0]]), free(pos), rstr);
}

/*
int main(void){
	char **rstr = ft_split("a b", ' ');
	int i = 0;
	while (rstr && rstr[i] != NULL)
	{
		printf("[%s]\n", rstr[i]);
		free(rstr[i]);
		i++;
	}
	free(rstr);
} */
