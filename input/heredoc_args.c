/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:12:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 19:20:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	append_command_arg(char **args, char **av, int *i, int *j)
{
	args[*j] = ft_strdup(av[*i]);
	if (!args[*j])
	{
		args[*j] = NULL;
		return (0);
	}
	(*j)++;
	(*i)++;
	return (1);
}

char	**collect_command_args(char **av, int ac, int *out_ac)
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
		if (is_redirect_token(av[i]))
		{
			i += 2;
			continue ;
		}
		if (!append_command_arg(args, av, &i, &j))
			return (free_arr((void **)args), NULL);
	}
	args[j] = NULL;
	*out_ac = j;
	return (args);
}
