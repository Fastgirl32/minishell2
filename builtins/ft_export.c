/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:45 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/08 13:28:31 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
input wie export var=ex und var="ex" und var=""ex" (quote) handeln plz
ich handle nur geparsten string wie export var=ex.
*/

void	declare(char *key, char *val)
{
	if (val && key)
		printf("declare -x %s=\"%s\"\n", key, val);
	else if (key)
		printf("declare -x %s\n", key);
	free(key);
	free(val);
}

t_status	ft_export(t_command *cmd, t_vars *vars)
{
	int		i;
	char	*key_;
	char	*val_;

	i = 0;
	if (cmd->ac == 1)
	{
		while (vars->env && (vars->env)[i] && (vars->env)[i][0])
		{
			key_ = key((vars->env)[i]);
			val_ = value((vars->env)[i]);
			declare(key_, val_);
			i++;
		}
	}
	else
	{
		while ((i + 1) < cmd->ac)
		{
			env_append(vars, cmd->argv[i + 1]);
			i++;
		}
	}
	return (0);
}
