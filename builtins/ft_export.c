/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 14:05:45 by lstarek           #+#    #+#             */
/*   Updated: 2026/05/07 14:05:46 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
input wie export var=ex und var="ex" und var=""ex" (quote) handeln plz
ich handle nur geparsten string wie export var=ex.
*/
t_status	ft_export(t_command *cmd, char ***env)
{
	int		i;
	char	*key_;
	char	*val_;

	i = 0;
	if (cmd->ac == 0)
	{
		while (*env
			&& (*env)[i] != NULL
			&& (*env)[i][0] != '\0')
		{
			key_ = key((*env)[i]);
			val_ = value((*env)[i]);
			printf("declare -x %s=\"%s\"\n", key_, val_);
			free(key_);
			free(val_);
			i++;
		}
	}
	else
	{
		while ((i + 1) < cmd->ac)
		{
			env_append(env, cmd->argv[i + 1]);
			i++;
		}
	}
	return (0);
}
