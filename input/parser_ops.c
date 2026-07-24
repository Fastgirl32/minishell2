#include "minishell.h"

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

int	append_redirect_nodes(struct s_redir *rd)
{
	t_command	*cmd;

	cmd = *rd->tail;
	while (rd->op_i + 1 < rd->ac && rd->av[rd->op_i]
		&& is_redirect_op(rd->av[rd->op_i]))
	{
		if (!set_command_limiter(cmd, rd->av[rd->op_i]))
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
	if (rd->op_i < 0 || rd->op_i + 1 >= rd->ac)
		return (0);
	if (rd->op_i == 0)
		return (consume_redir_only_segment(rd));
	left_av = dup_token_range(rd->av, 0, rd->op_i, &left_ac);
	if (!left_av)
		return (-1);
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
