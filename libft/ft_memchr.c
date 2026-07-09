/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 21:46:27 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:41:36 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ss;
	unsigned char		cc;
	size_t				i;

	ss = (const unsigned char *)s;
	cc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ss[i] == cc)
			return ((void *)ss + i);
		i++;
	}
	return (NULL);
}

/*
int main(){
char *karre = "hello";
ft_memchr(karre, 90, 5);} */
