/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:18:12 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/02 11:51:36 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_pipe_limiter(t_command *cmd, int has_pipe)
{
	if (cmd->limiter || !has_pipe)
		return (1);
	cmd->limiter = ft_strdup("|");
	if (!cmd->limiter)
		return (0);
	return (1);
}

int	fill_command_base(t_command *cmd, char **av, int *ac, int has_pipe)
{
	if (*ac < 1 || !av[0])
		return (0);
	cmd->command = ft_strdup(av[0]);
	if (!cmd->command)
		return (0);
	cmd->limiter = NULL;
	if (!set_pipe_limiter(cmd, has_pipe))
		return (free(cmd->command), free(cmd->limiter), 0);
	cmd->ac = *ac;
	if (!ft_strcmp(av[0], "exit") && *ac == 1)
		cmd->ac = 0;
	return (1);
}

char	**dup_token_range(char **av, int start, int end, int *out_ac)
{
	char	**copy;
	int		i;
	int		j;

	if (end <= start)
		return (NULL);
	copy = malloc(sizeof(char *) * (size_t)(end - start + 1));
	if (!copy)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		copy[j] = ft_strdup(av[i]);
		if (!copy[j])
			return (copy[j] = NULL, free_arr((void **)copy), NULL);
		i++;
		j++;
	}
	copy[j] = NULL;
	if (out_ac)
		*out_ac = j;
	return (copy);
}

t_command	*new_command(char **av, int ac, int has_pipe)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	if (!fill_command_base(cmd, av, &ac, has_pipe))
		return (free(cmd), NULL);
	cmd->argv = av;
	cmd->next = NULL;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->redir_failed = 0;
	return (cmd);
}

int	first_redir_index(char **av, int *is_op, int ac)
{
	int	i;

	i = 0;
	while (i < ac && !(is_op[i] && is_redirect_op(av[i])))
		i++;
	if (i >= ac)
		return (-1);
	return (i);
}
