/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:36:50 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 12:50:52 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	strilen(char const *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*rstr;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	rstr = malloc(strilen(s) + 1);
	if (rstr == NULL)
		return (NULL);
	i = 0;
	while (s[i] != 0)
	{
		rstr[i] = f((unsigned int)i, s[i]);
		i++;
	}
	rstr[i] = 0;
	return (rstr);
}

/*
char increment(unsigned int x, char str)
{
	(void)x;
	return (str + 1);
}

#include <stdio.h>
int main(){
	char *x = "test faden";
	printf("%s\n",  ft_strmapi(x, increment));
} */
