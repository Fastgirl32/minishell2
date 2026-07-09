/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/07 17:40:10 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int	abso(int nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}

int	ft_putnbr(int n)
{
	char	*map;
	int		i;

	i = 0;
	map = "0123456789";
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	else
	{
		if (abso(n) > 9)
		{
			i += ft_putnbr(n / 10);
			i += write(1, &map[abso(n % 10)], 1);
		}
		else
		{
			if (n < 0)
				i += write(1, "-", 1);
			i += write(1, &map[abso(n)], 1);
		}
	}
	return (i);
}
