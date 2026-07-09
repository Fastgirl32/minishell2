/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:11:05 by lstarek           #+#    #+#             */
/*   Updated: 2025/08/24 13:27:48 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if (!(97 <= c && c <= 122)
		&& !(65 <= c && c <= 90)
		&& !(48 <= c && c <= 57))
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
		{if (!((isalnum(i)==0) != (ft_isalnum(i))))
			{printf("%d, %d, incorrect!\n", isalnum(i) , ft_isalnum(i));}}
} */
