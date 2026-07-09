/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:17:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/20 18:10:50 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*csrc;
	unsigned char	*cdest;

	if (dest == NULL && src == NULL)
		return (NULL);
	csrc = (unsigned char *)src;
	cdest = (unsigned char *)dest;
	if (dest < src)
	{
		while (n != 0)
		{
			*cdest++ = *csrc++;
			n--;
		}
	}
	else
	{
		while (n != 0)
		{
			cdest[n - 1] = csrc[n - 1];
			n--;
		}
	}
	return (dest);
}
