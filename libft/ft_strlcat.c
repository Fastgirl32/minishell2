/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/21 14:52:42 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	d_len = 0;
	s_len = 0;
	while ((dst[d_len] != 0) && (d_len < size))
		d_len++;
	while (src[s_len] != 0)
		s_len++;
	if (d_len == size)
		return (size + s_len);
	i = 0;
	while ((src[i] != 0) && ((d_len + i + 1) < size))
	{
		dst[d_len + i] = src[i];
		i++;
	}
	dst[d_len + i] = 0;
	return (d_len + s_len);
}

/*
#include <stdio.h>
int main(){
	char x[99] = "faden";
	printf("%ld, %s\n", ft_strlcat(x, "katze", 11), x);
} */
