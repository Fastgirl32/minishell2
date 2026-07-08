/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:55 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:56 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	ft_unset(t_command *cmd, char ***env)
{
	int	i;

	i = 0;
	if (cmd->ac < 1)
	{
		ft_putstr_fd("unset: not enough arguments\n", STDERR_FILENO);
		return (1);
	}
	while ((i + 1) < cmd->ac)
	{
		env_remove(env, cmd->argv[i + 1]);
		i++;
	}
	return (0);
}
