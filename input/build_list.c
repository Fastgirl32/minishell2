#include "../minishell.h"

int	should_skip_list(t_command *head, t_vars *vars)
{
	if (!head->next && head->command && head->command[0] == '\0')
		return (1);
	if (!head->next && !ft_strcmp(head->command, "history"))
	{
		history_print(vars);
		return (1);
	}
	return (0);
}

t_command	*build_command_list(t_vars *vars, const char *line)
{
	struct s_redir	rd;
	t_command		*head;
	t_command		*tail;
	size_t			i;

	head = NULL;
	tail = NULL;
	rd.head = &head;
	rd.tail = &tail;
	rd.vars = vars;
	i = skip_blanks(line, 0);
	if (line[i] == '#')
		return (NULL);
	while (line[i] && line[i] != '\n')
		i = parse_and_move(&rd, line, i, vars->status);
	return (head);
}

void	make_list(t_vars *vars, char *line)
{
	t_command	*head;

	head = build_command_list(vars, line);
	vars->list = head;
	if (!head || should_skip_list(head, vars))
		return (free_list(head), (void)(vars->list = NULL));
	if (!connect_pipes(head) && !prepare_heredocs(head)
		&& !establish_redirects(head))
	{
		print_command_list(head);
		if (head->next)
			execute(head, vars);
		else
			execute_single_command(head, vars);
	}
	free_list(head);
	vars->list = NULL;
}

/*
frees a t_command linked list.
*/
void	free_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		free(cmd->command);
		free(cmd->limiter);
		free_arr((void **)cmd->argv);
		free(cmd);
		cmd = next;
	}
}