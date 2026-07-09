/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/09/30 14:54:38 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (!(48 <= c && c <= 57))
		return (0);
	return (1);
}

/*
#include <stdio.h>
#include <ctype.h>
//tests if both original and my version return a non-zero value at the same time.
//if correct, nothing is printed.
//to test, replace the != with ==
int main(void){
	for (int i = 0; i <= 126; i++)
		{if (!((isdigit(i)==0) != (ft_isdigit(i))))
			{printf("%d, %d, incorrect!\n", isdigit(i) , ft_isdigit(i));}}
} */
