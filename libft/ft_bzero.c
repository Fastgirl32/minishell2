/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:09:23 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/08 15:49:40 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}

/*
#include <stdio.h>
#include <stdlib.h>
int main(){
	char *x = malloc(5);
	ft_bzero(x, 5);
	for (int i = 0; i < 5; i++)
		printf("is zero: %d\n", x[i]==0);
} */
