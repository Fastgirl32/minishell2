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

	(void)cmd;
	getcwd(path, PATH_MAX);
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}
