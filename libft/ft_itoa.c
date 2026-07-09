/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/20 17:12:24 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	numlen(int n)
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

char	*ft_itoa(int n)
{
	char		*str;
	int			len;
	long int	nb;

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
printf("%s\n", ft_itoa(0));
printf("%s\n", ft_itoa(420));
printf("%s\n", ft_itoa(-2147483648));
printf("%s\n", ft_itoa(-420));} */
