/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:05:00 by saecker           #+#    #+#             */
/*   Updated: 2026/09/02 12:56:51 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*new_single_arg_command(char *arg)
{
	char		**av;
	t_command	*cmd;

	av = malloc(sizeof(char *) * 2);
	if (!av)
		return (NULL);
	av[0] = ft_strdup(arg);
	if (!av[0])
		return (free(av), NULL);
	av[1] = NULL;
	cmd = new_command(av, 1, 0);
	if (!cmd)
		free_arr((void **)av);
	return (cmd);
}

char	**copy_command_args(char **av, int *is_op, int ac, int *out_ac)
{
	char	**args;
	int		i;
	int		j;

	args = malloc(sizeof(char *) * (size_t)(ac + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ac)
	{
		if (is_op[i] && is_redirect_op(av[i]))
			i += 2;
		else
		{
			args[j] = ft_strdup(av[i]);
			if (!args[j])
				return (args[j] = NULL, free_arr((void **)args), NULL);
			j++;
			i++;
		}
	}
	args[j] = NULL;
	*out_ac = j;
	return (args);
}
