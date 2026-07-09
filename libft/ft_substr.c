/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:37:19 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:56:01 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	strl(char const *str)
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
	if (str == NULL)
		return (NULL);
	else
		str[0] = '\0';
	return (str);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	substr_len;
	char	*sstr;
	size_t	i;

	if (s == NULL)
		return (NULL);
	s_len = strl(s);
	if (start > s_len)
		return (emptystr());
	substr_len = len;
	if (len > (s_len - start))
		substr_len = s_len - start;
	sstr = malloc(substr_len + 1);
	if (sstr == NULL)
		return (NULL);
	i = 0;
	while (i < substr_len)
	{
		sstr[i] = s[start + i];
		i++;
	}
	sstr[i] = 0;
	return (sstr);
}

/*
#include <stdio.h>
int main(void)
{
	printf("%s\n", ft_substr("i really do", 3, 5));
} */
