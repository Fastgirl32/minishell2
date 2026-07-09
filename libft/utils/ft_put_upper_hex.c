/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_upper_hex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:25:18 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/07 17:41:05 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../ft_printf.h"

int	ft_put_upper_hex(unsigned int address)
{
	char	*base;
	int		i;

	i = 0;
	base = "0123456789ABCDEF";
	if (address >= 16)
	{
		i += ft_put_upper_hex(address / 16);
		i += write(1, &base[address % 16], 1);
	}
	else
	{
		i += write(1, &base[address], 1);
	}
	return (i);
}
