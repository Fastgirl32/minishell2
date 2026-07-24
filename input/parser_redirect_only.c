#include "minishell.h"

int	consume_redir_only_segment(struct s_redir *rd)
{
	t_command	*heredoc;

	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac && is_redirect_op(rd->av[rd->op_i]))
		rd->op_i += 2;
	if (rd->op_i != rd->ac)
		return (0);
	rd->op_i = 0;
	while (rd->op_i + 1 < rd->ac)
	{
		if (!ft_strcmp(rd->av[rd->op_i], "<<"))
		{
			heredoc = build_heredoc(rd->av[rd->op_i + 1]);
			if (!heredoc)
				return (-1);
			free_list(heredoc);
		}
		rd->op_i += 2;
	}
	return (1);
}

int	has_pipe_after_segment(const char *line, size_t end)
{
	while (line[end] && is_blank(line[end]))
		end++;
	if (line[end] == '|')
		return (1);
	return (0);
}
