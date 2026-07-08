/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:40 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:41 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	status2_message(t_command *cmd)
{
	ft_putstr_fd("exit: ", cmd->fd_out);
	ft_putstr_fd(cmd->argv[0], cmd->fd_out);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (2);
}

t_status	ft_exit(t_command *cmd)
{
	t_status	status;
	char		*numberified;

	if (cmd->ac <= 2)
		status = 0;
	if (cmd->ac > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);
	}
	if (cmd->ac > 0)
	{
		status = (t_status)(ft_atoi(cmd->argv[1]) % 256);
		numberified = ft_itoa(ft_atoi(cmd->argv[1]));
		if (ft_strcmp(numberified, cmd->argv[1]))
			status = status2_message(cmd);
		free(numberified);
	}
	ft_putstr_fd("exit\n", 1);
	exit(status);
}
