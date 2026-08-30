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

#include "../minishell.h"

static int	is_it_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static int	numlen(long int n)
{
	int	len;

	len = (n <= 0);
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

long int	ft_atol(const char *str)
{
	long int	i;
	long int	minus_counter;
	long int	sum;

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

char	*ft_ltoa(long int n)
{
	char			*str;
	int				len;
	long long int	nb;

	nb = n;
	len = numlen(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		str[--len] = '0';
	while (nb > 0)
	{
		str[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}

/*
#include <stdio.h>
int main(){
 printf("%d\n", ft_atoi("2147483648"));
 printf("%d\n", ft_atoi("420"));
 printf("%d\n", ft_atoi("-420"));
 printf("%d\n", ft_atoi("0"));
 } */
