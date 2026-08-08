/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:27 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/08 13:29:34 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	ft_env(t_command *cmd, t_vars *vars)
{
	int		i;
	char	*val;

	i = 0;
	if (cmd->ac > 1)
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
