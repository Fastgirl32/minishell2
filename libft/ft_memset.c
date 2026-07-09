/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:13:29 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/20 18:44:56 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ss;

	i = 0;
	ss = (unsigned char *)s;
	while (i < n)
	{
		ss[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*
#include <stdio.h>
#include <stdlib.h>
int main(){
	char *x = malloc(5);
	ft_memset(x, 'e' ,5);
	for (int i = 0; i < 5; i++)
		printf("%c\n", x[i]);
} */
