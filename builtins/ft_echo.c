/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:19 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/05 10:24:20 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_u16	is_n_flag(char *str)
{
	size_t	c;

	c = 0;
	while (str[c])
	{
		if (c == 0 && str[c] != '-')
			return (0);
		if (c > 0 && str[c] != 'n')
			return (0);
		c++;
	}
	if (c == 0)
		return (0);
	return (1);
}

t_status	ft_echo(t_command *cmd, char **env)
{
	t_u16	flag_n;
	int		i;

	ft_printf("curstom\n");
	(void)env;
	flag_n = 0;
	i = 0;
	while (cmd->argv[1] && i < cmd->ac && is_n_flag(cmd->argv[i + 1]))
	{
		flag_n = 1;
		i++;
	}
	while (cmd->argv[1] && (i + 1) < cmd->ac)
	{
		print_env_fd(1, cmd->argv[i + 1], env);
		if (i < (cmd->ac - 1))
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!flag_n)
		ft_putstr_fd("\n", 1);
	return (0);
}
