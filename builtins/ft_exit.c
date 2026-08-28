/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:40 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/28 16:23:30 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	status2_message(t_command *cmd)
{
	ft_printf("exit: %s: numeric argument required\n", cmd->argv[1]);
	return (2);
}

t_status	ft_exit(t_command *cmd, t_vars *vars)
{
	t_status	status;
	char		*numberified;

	if (cmd->ac <= 2)
		status = 0;
	if (cmd->ac > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (cmd->ac > 0)
	{
		status = (t_status)(ft_atol(cmd->argv[1]) % 256);
		numberified = ft_ltoa(ft_atol(cmd->argv[1]));
		if (ft_strcmp(numberified, cmd->argv[1]))
			status = status2_message(cmd);
		free(numberified);
	}
	vars->stop = 1;
	return (status);
}
