/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:56:57 by lstarek           #+#    #+#             */
/*   Updated: 2025/09/03 23:03:13 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	leng(char *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		c;

	dest = malloc(leng(src)+1);
	if (dest == NULL)
		return (NULL);
	c = 0;
	while (src[c] != 0)
	{
		dest[c] = src[c];
		c++;
	}
	dest[c] = 0;
	return (dest);
}

/*
#include <stdio.h>
int main(){ 
char *result = ft_strdup("teststring");
printf("%s\n", result);
//free(result);
} */
