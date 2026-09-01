/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:22 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:25:23 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_command_limiter(t_command *cmd, char *lim)
{
	free(cmd->limiter);
	cmd->limiter = ft_strdup(lim);
	if (!cmd->limiter)
		return (0);
	return (1);
}

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

static char	**copy_command_args(char **av, int ac, int *out_ac)
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
		if (is_redirect_op(av[i]))
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

int	append_redirect_nodes(struct s_redir *rd)
{
	t_command	*cmd;

	cmd = *rd->tail;
	rd->op_i = 0;
	while (rd->op_i < rd->ac)
	{
		if (!is_redirect_op(rd->av[rd->op_i]))
		{
			rd->op_i++;
			continue ;
		}
		if (rd->op_i + 1 >= rd->ac
			|| !set_command_limiter(cmd, rd->av[rd->op_i]))
			return (0);
		if (!ft_strcmp(rd->av[rd->op_i], "<<"))
			cmd = build_heredoc(rd->av[rd->op_i + 1]);
		else
			cmd = new_single_arg_command(rd->av[rd->op_i + 1]);
		if (!cmd)
			return (0);
		append_command(rd->head, rd->tail, cmd);
		rd->op_i += 2;
	}
	return (1);
}

static int	set_pipe_for_segment(struct s_redir *rd)
{
	if (rd->has_pipe && *rd->tail && !(*rd->tail)->limiter
		&& !set_command_limiter(*rd->tail, "|"))
		return (0);
	return (1);
}

int	handle_redirect_segment(struct s_redir *rd)
{
	t_command	*cmd;
	char		**left_av;
	int			left_ac;

	rd->op_i = first_redir_index(rd->av, rd->ac);
	if (rd->op_i < 0)
		return (0);
	left_av = copy_command_args(rd->av, rd->ac, &left_ac);
	if (!left_av)
		return (-1);
	if (left_ac == 0)
		return (free_arr((void **)left_av),
			consume_redir_only_segment(rd));
	cmd = new_command(left_av, left_ac, 0);
	if (!cmd)
		return (free_arr((void **)left_av), -1);
	append_command(rd->head, rd->tail, cmd);
	if (!append_redirect_nodes(rd))
		return (-1);
	if (!set_pipe_for_segment(rd))
		return (-1);
	return (1);
}
