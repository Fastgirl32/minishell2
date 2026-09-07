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

/*
Translates the info in "status" into a usable exit code.
*/
int	exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

t_status	numericargs_message(t_command *cmd)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(cmd->argv[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (2);
}

t_status	toomanyargs_message(t_command *cmd)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(cmd->argv[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (2);
}

t_status	ft_exit(t_command *cmd, t_vars *vars)
{
	t_status	status;
	char		*numberified;

	if (cmd->ac < 2)
	{
		vars->stop = 1;
		return (0);
	}
	else
	{
		status = (t_status)(ft_atol(cmd->argv[1]) % 256);
		numberified = ft_ltoa(ft_atol(cmd->argv[1]));
		if (ft_strcmp(numberified, cmd->argv[1]))
			status = numericargs_message(cmd);
		else if (cmd->ac > 2)
			status = toomanyargs_message(cmd);
		free(numberified);
	}
	vars->stop = 1;
	return (status);
}
