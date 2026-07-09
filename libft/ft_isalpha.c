/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/09/30 14:54:11 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if (!(97 <= c && c <= 122)
		&& !(65 <= c && c <= 90))
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
		{if (!((isalpha(i)==0) != (ft_isalpha(i))))
			{printf("%d, %d, incorrect!\n", isalpha(i) , ft_isalpha(i));}}
} */
