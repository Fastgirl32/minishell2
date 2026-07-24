#include "minishell.h"

int	g_status = 0;

void	free_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->fd_in > 2)
			close(cmd->fd_in);
		if (cmd->fd_out > 2)
			close(cmd->fd_out);
		free(cmd->command);
		free(cmd->limiter);
		free_arr((void **)cmd->argv);
		free(cmd);
		cmd = next;
	}
}

void	execute_single_command(t_command *head, t_vars *vars)
{
	pid_t	child_pid;

	if (is_builtin(head->command))
		return ((void)execute_builtin(head, vars->env));
	child_pid = fork();
	if (!child_pid)
	{
		setup_child_signals();
		if (head->fd_in != 0)
		{
			dup2(head->fd_in, 0);
			close(head->fd_in);
		}
		if (head->fd_out != 1)
		{
			dup2(head->fd_out, 1);
			close(head->fd_out);
		}
		find_and_exec(head, recreate_env(vars->env));
	}
	else
		waitpid(child_pid, NULL, 0);
}

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
		i = parse_and_move(&rd, line, i);
	return (head);
}

void	make_list(t_vars *vars, char *line)
{
	t_command	*head;

	head = build_command_list(vars, line);
	vars->list = head;
	print_command_list(head);
	if (!head || should_skip_list(head, vars))
		return (free_list(head), (void)(vars->list = NULL));
	if (!connect_pipes(head) && !prepare_heredocs(head)
		&& !establish_redirects(head))
	{
		if (head->next)
			execute(head, vars->env);
		else
			execute_single_command(head, vars);
	}
	free_list(head);
	vars->list = NULL;
}
