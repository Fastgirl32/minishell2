/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:22 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:33:37 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Creates command nodes for all redirections in a segment. */
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
			return (-1);
		if (!ft_strcmp(rd->av[rd->op_i] + 1, "<<"))
			cmd = build_heredoc(rd->av[rd->op_i + 1]);
		else
			cmd = new_single_arg_command(rd->av[rd->op_i + 1]);
		if (!cmd)
			return (-1);
		append_command(rd->head, rd->tail, cmd);
		rd->op_i += 2;
	}
	return (1);
}

/* Adds a pipe marker to the last command when required. */
static int	set_segment_pipe(struct s_redir *rd)
{
	if (rd->has_pipe && *rd->tail && !(*rd->tail)->limiter
		&& !set_command_limiter(*rd->tail, "|"))
		return (0);
	return (1);
}

/* Builds a command and attaches its redirection nodes. */
static int	build_redirect_command(struct s_redir *rd, char **args, int ac)
{
	t_command	*cmd;

	cmd = new_command(args, ac, 0);
	if (!cmd)
		return (-1);
	append_command(rd->head, rd->tail, cmd);
	if (!append_redirect_nodes(rd) || !set_segment_pipe(rd))
		return (-1);
	return (1);
}

/* Chooses the correct builder for a redirected segment. */
int	handle_redirect_segment(struct s_redir *rd)
{
	char		**left_av;
	int			left_ac;

	rd->op_i = first_redir_index(rd->av, rd->ac);
	if (rd->op_i < 0)
		return (0);
	left_av = copy_command_args(rd->av, rd->ac, &left_ac);
	if (!left_av)
		return (-1);
	if (left_ac == 0 || (rd->has_pipe && left_ac == 1
			&& !ft_strcmp(left_av[0], "cat")
			&& !ft_strcmp(rd->av[rd->op_i], "<<")))
	{
		free_arr((void **)left_av);
		if (!rd->has_pipe)
			return (consume_redir_only_segment(rd));
		return (append_cat_heredoc(rd));
	}
	return (build_redirect_command(rd, left_av, left_ac));
}
