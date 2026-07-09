/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:54:33 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 13:13:04 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_it_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	minus_counter;
	int	sum;

	i = 0;
	minus_counter = 0;
	sum = 0;
	while (is_it_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		minus_counter = (str[i] == '-');
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		sum = (sum * 10) + (str[i] - 48);
		i++;
	}
	if (minus_counter == 1)
		return (-sum);
	else
		return (sum);
}

/*
#include <stdio.h>
int main(){
 printf("%d\n", ft_atoi("2147483648"));
 printf("%d\n", ft_atoi("420"));
 printf("%d\n", ft_atoi("-420"));
 printf("%d\n", ft_atoi("0"));
 } */
