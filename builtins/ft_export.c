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

void	declare(char *key, char *val, t_command *cmd)
{
	int		out;

	out = STDOUT_FILENO;
	if (cmd->is_single)
		out = cmd->fd_out;
	if (val || key)
		ft_putstr_fd("declare -x ", out);
	if (val && key)
	{
		ft_putstr_fd(key, out);
		ft_putstr_fd("=", out);
		ft_putstr_fd(val, out);
		ft_putstr_fd("\n", out);
	}
	else if (key)
	{
		ft_putstr_fd(key, out);
		ft_putstr_fd("\n", out);
	}
	free(key);
	free(val);
}

/*
The identifier must:
Start with a letter (a-z, A-Z) or underscore (_)
Continue with letters, digits, or underscores
Not contain -, ., spaces, etc.
Only looks until the =
*/
_Bool	identifier_valid(char *var)
{
	int	i;

	i = 1;
	if (!var || !var[0])
		return (0);
	if (!isalpha(var[0]) && var[0] != '_')
		return (0);
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_exported(t_vars *vars, t_command *cmd)
{
	char	*key_;
	char	*val_;
	int		i;

	i = 0;
	while (vars->env && (vars->env)[i] && (vars->env)[i][0])
	{
		key_ = key((vars->env)[i]);
		val_ = value((vars->env)[i]);
		declare(key_, val_, cmd);
		i++;
	}
}

t_status	ft_export(t_command *cmd, t_vars *vars)
{
	int		i;
	int		failed;

	i = 0;
	failed = 0;
	if (cmd->ac == 1)
		print_exported(vars, cmd);
	else
	{
		while ((i + 1) < cmd->ac)
		{
			if (identifier_valid(cmd->argv[i + 1]))
				env_append(vars, cmd->argv[i + 1]);
			else
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->argv[i + 1], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				failed = 1;
			}
			i++;
		}
	}
	return (failed);
}
