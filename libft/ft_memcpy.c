/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:17:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:42:20 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*csrc;
	unsigned char	*cdest;

	if (dest == NULL && src == NULL)
		return (NULL);
	csrc = (unsigned char *)src;
	cdest = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		cdest[i] = csrc[i];
		i++;
	}
	return (cdest);
}

/*
#include <stdio.h>
int main(void){
char baal[99];
printf("%s\n", (char *)ft_memcpy(baal, "whuaaa", 5));} */
