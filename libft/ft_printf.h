/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:40:00 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/07 17:45:42 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef long unsigned int		t_lui;
typedef long long unsigned int	t_llui;

int								ft_printf(const char *s, ...);
int								ft_putstr(char *s);
int								ft_putnbr(int n);
int								ft_putchar(char c);
int								ft_putptr(void *ptr);
int								ft_put_low_hex(unsigned int address);
int								ft_put_upper_hex(unsigned int address);
int								ft_putuint(unsigned int n);

#endif
