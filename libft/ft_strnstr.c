/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:51:51 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	little_len;

	if (little[0] == '\0')
		return ((char *)big);
	little_len = 0;
	while (little[little_len] != 0)
		little_len++;
	i = 0;
	while ((big[i] != 0) && (i + little_len <= len))
	{
		j = 0;
		while ((j < little_len) && (big[i + j] == little[j]))
			j++;
		if (j == little_len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int main(void){
	printf("'%s'\n", ft_strnstr("lorem ipsum dolor sit amet", "dolor", 15));
} */
