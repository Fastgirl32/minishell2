/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:36:41 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:49:59 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	strilen(char const *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*rstr;
	size_t		leng;
	size_t		i;

	i = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	leng = strilen(s1) + strilen(s2);
	rstr = malloc(leng + 1);
	if (rstr == NULL)
		return (NULL);
	while (*s1 != 0)
		rstr[i++] = *s1++;
	while (*s2 != 0)
		rstr[i++] = *s2++;
	rstr[i] = 0;
	return (rstr);
}

/*
#include <stdio.h>
int main(void){
	printf("%s\n", ft_strjoin("hello","world"));
} */
