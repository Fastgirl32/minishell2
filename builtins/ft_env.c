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

t_status	ft_env(t_command *cmd, t_vars *vars)
{
	int		i;
	char	*val;

	i = 0;
	if (cmd->ac > 1)
//ändern, stimmt ned
		return ((void)printf("env: too many arguments\n"), 2);
	while (vars->env[i])
	{
		val = value(vars->env[i]);
		if (val)
			printf("%s\n", vars->env[i]);
		free(val);
		i++;
	}
	return (0);
}
