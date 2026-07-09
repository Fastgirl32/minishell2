/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/10/20 18:47:48 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int	abso(int nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	*map;

	map = "0123456789";
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (abso(n) > 9)
		{
			ft_putnbr_fd(n / 10, fd);
			write(fd, &map[abso(n % 10)], 1);
		}
		else
		{
			if (n < 0)
				write(fd, "-", 1);
			write(fd, &map[abso(n)], 1);
		}
	}
}
