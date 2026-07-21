/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:28 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	ft_env(t_command *cmd, char **env)
{
	if (cmd->ac)
//ändern, stimmt ned
		return ((void)printf("env: too many arguments\n"), 2);
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}
