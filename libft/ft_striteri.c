/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:36:34 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/21 15:09:19 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (s == NULL || f == NULL)
		return ;
	i = 0;
	while (s[i] != 0)
	{
		f(i, &s[i]);
		i++;
	}
}

/*
void increment(unsigned int x, char *str)
{
	(void)x;
	*str += 1;
}

#include <stdio.h>
int main(){
	char x[] = "test faden";
	ft_striteri(x, increment);
	printf("%s\n",  x);
} */
