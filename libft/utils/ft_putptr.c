/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/07 17:40:39 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../ft_printf.h"

int	compute(t_lui address)
{
	char	*base;
	int		i;

	if (address == 0)
		return (write(1, "(nil)", 5));
	i = 0;
	base = "0123456789abcdef";
	if (address >= 16)
	{
		i += compute(address / 16);
		i += write(1, &base[address % 16], 1);
	}
	else
	{
		i += write(1, "0x", 2);
		i += write(1, &base[address], 1);
	}
	return (i);
}

int	ft_putptr(void *ptr)
{
	return (compute((t_lui)ptr));
}
