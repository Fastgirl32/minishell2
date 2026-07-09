/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 21:46:12 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/20 18:51:10 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strchr(const char *str, int c)
{
	while (*str != 0)
	{
		if ((unsigned char)(*str) == (unsigned char)c)
			return ((char *)str);
		str++;
	}
	if ((unsigned char)*(str) == (unsigned char)c)
		return ((char *)str);
	return (NULL);
}

/*
#include <stdio.h>
int main(){
	printf("%p\n",ft_strchr("teste", '\0'));
} */
