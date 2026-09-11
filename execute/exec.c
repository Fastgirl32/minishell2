/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:07:13 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/11 09:26:09 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Executes a built-in directly in the current process.
In a pipeline, this will not be the root process.
In a single command, it is.
*/
int	execute_builtin(t_command *cmd, t_vars *vars)
{
	if (!ft_strcmp(cmd->command, "echo"))
		return (ft_echo(cmd, vars));
	else if (!ft_strcmp(cmd->command, "cd"))
		return (ft_cd(cmd, vars));
	else if (!ft_strcmp(cmd->command, "pwd"))
		return (ft_pwd(cmd));
	else if (!ft_strcmp(cmd->command, "export"))
		return (ft_export(cmd, vars));
	else if (!ft_strcmp(cmd->command, "unset"))
		return (ft_unset(cmd, vars));
	else if (!ft_strcmp(cmd->command, "env"))
		return (ft_env(cmd, vars));
	else if (!ft_strcmp(cmd->command, "exit"))
		return (ft_exit(cmd, vars));
	return (1);
}

/*
executes a command that is not part of a pipeline.
If it is builtin, it executes directly in the main process.
Otherwise, it forks once and runs the program in the child.
*/
void	execute_single_command(t_command *cmd, t_vars *vars)
{
	pid_t	child_pid;
	int		stat;

	cmd->is_single = 1;
	if (is_builtin(cmd->command))
	{
		*(vars->status) = execute_builtin(cmd, vars);
		return ((void)ft_close(&cmd->fd_in), ft_close(&cmd->fd_out));
	}
	if (!print_heredoc(cmd))
		return ;
	child_pid = fork();
	if (!child_pid)
	{
		if (redirect_all(cmd))
			exit(1);
		find_and_exec(cmd, vars);
	}
	else
	{
		ignore_parent_signals();
		waitpid(child_pid, &stat, 0);
		setup_parent_signals();
		*(vars->status) = exit_status(stat);
	}
}

void	perform_action(t_command *cmd, t_vars *vars)
{
	if (dup_all(cmd))
		exit(1);
	if (is_builtin(cmd->command))
		clean_exit(execute_builtin(cmd, vars), vars);
	if (!print_heredoc(cmd))
		clean_exit(0, vars);
	else
		find_and_exec(cmd, vars);
}

/*
Executes a pipeline recursively.
*/
void	execute(t_command *cmd, t_vars *vars)
{
	pid_t	child_pid;
	int		stat;

	if (!cmd)
		return ;
	child_pid = fork();
	cmd->is_single = 0;
	if (!child_pid)
	{
		setup_child_signals();
		perform_action(cmd, vars);
	}
	else
	{
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		ignore_parent_signals();
		execute(cmd->next, vars);
		waitpid(child_pid, &stat, 0);
		if (!(cmd->next))
			*(vars->status) = exit_status(stat);
		setup_parent_signals();
	}
}
