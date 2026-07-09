/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:35:40 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/13 17:50:09 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memory;
	size_t	total_size;
	size_t	i;

	if (nmemb == 0 || size == 0)
	{
		memory = malloc(1);
		if (!memory)
			return (NULL);
		((char *)memory)[0] = 0;
		return (memory);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	total_size = nmemb * size;
	memory = malloc(total_size);
	if (!memory)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		((char *)memory)[i] = 0;
		i++;
	}
	return (memory);
}

/*
int main(void){
	char *new = ft_calloc(100, 1);
	(void)new;
} */
