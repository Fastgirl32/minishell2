/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:51 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:52 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_status	ft_pwd(t_command *cmd)
{
	char	path[PATH_MAX];

	if (cmd->argv[1])
	{
		ft_putstr_fd("pwd: too many arguments\n", 1);
		return (1);
	}
	getcwd(path, PATH_MAX);
	ft_putstr_fd(path, 0);
	ft_putstr_fd("\n", 0);
	return (0);
}
