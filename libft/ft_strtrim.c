/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:37:12 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:53:08 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static int	is_in(const char c, const char *str)
{
	if (!str)
		return (0);
	while (*str != 0)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

static size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	if (dst == NULL || src == NULL)
		return (0);
	src_len = 0;
	while (src[src_len] != 0)
		src_len++;
	if (size == 0)
		return (src_len);
	i = 0;
	while ((src[i] != 0) && (i < (size - 1)))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (src_len);
}

static size_t	strl(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

static char	*emptystr(void)
{
	char	*str;

	str = malloc(1);
	if (!str)
		return (NULL);
	*str = 0;
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*rstr;
	char	*orig;
	size_t	a;
	size_t	e;

	if (s1 == NULL)
		return (NULL);
	if (*s1 == 0)
		return (emptystr());
	a = 0;
	e = strl(s1);
	orig = (char *)s1;
	while (is_in(*s1++, set))
		a++;
	while (*s1 != 0)
		s1++;
	s1--;
	while (is_in(*s1--, set))
		e--;
	rstr = malloc((e - a) + 1);
	if (rstr == NULL)
		return (free(rstr), NULL);
	ft_strlcpy(rstr, orig + a, e - a + 1);
	return (rstr);
}

/*
#include <stdio.h>
int main(void){
	char	*no = ft_strtrim("", "cdef");
	printf("\n'%s'\n", no);
	free(no);
	return 0;
} */
