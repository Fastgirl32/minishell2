/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:19 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/09 18:43:43 by baal             ###   ########.fr       */
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

	flag_n = 0;
	(void)env;
	i = 0;
	while (cmd->argv[1] && i < cmd->ac && is_n_flag(cmd->argv[i + 1]))
	{
		flag_n = 1;
		i++;
	}
	while (cmd->argv[1] && (i + 1) < cmd->ac)
	{
		ft_printf(cmd->argv[i + 1]);
		if (i < (cmd->ac - 1))
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!flag_n)
		ft_printf("\n");
	return (0);
}
