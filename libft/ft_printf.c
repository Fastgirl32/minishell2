/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:14:22 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/04 14:38:56 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stddef.h>

static int	substitute(char c, va_list args)
{
	if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (c == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (c == 'p')
		return (ft_putptr(va_arg(args, void *)));
	else if (c == 'x')
		return (ft_put_low_hex(va_arg(args, t_llui)));
	else if (c == 'X')
		return (ft_put_upper_hex(va_arg(args, t_llui)));
	if (c == 'u')
		return (ft_putuint(va_arg(args, unsigned int)));
	else
		return (0);
}

int	ft_printf(const char *s, ...)
{
	va_list		args;
	size_t		bytes_printed;

	bytes_printed = 0;
	va_start(args, s);
	while (*s != 0)
	{
		if (*s != '%')
			bytes_printed += write(1, s, 1);
		else if (*(s + 1) == '%')
		{
			bytes_printed += write(1, "%", 1);
			s++;
		}
		else
		{
			s++;
			bytes_printed += substitute(s[0], args);
		}
		s++;
	}
	va_end(args);
	return (bytes_printed);
}

/*
#include <stdio.h>
int main(void){
int x = ft_printf("null str: %s\n", NULL);
int y = printf("null str: %s\n", NULL);
printf("%d ?= %d \n", x, y);
} */
