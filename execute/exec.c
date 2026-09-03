/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:07:13 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/30 12:49:18 by baal             ###   ########.fr       */
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
For a given command. dups all file descriptors
returns 0 on success
*/
t_status	redirect_all(t_command *cmd)
{
	setup_child_signals();
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, 0) == -1)
			return (ft_close(&cmd->fd_in), 1);
		ft_close(&cmd->fd_in);
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, 1) == -1)
			return (ft_close(&cmd->fd_out), 1);
		ft_close(&cmd->fd_out);
	}
	return (0);
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

	if (is_builtin(cmd->command))
	{
		*(vars->status) = execute_builtin(cmd, vars);
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		return ;
	}
	child_pid = fork();
	if (!child_pid)
	{
		if (redirect_all(cmd))
			exit(1);
		find_and_exec(cmd, vars);
	}
	else
	{
		waitpid(child_pid, &stat, 0);
		*(vars->status) = exit_status(stat);
	}
}

/*
For pipeline command. dups all file descriptors
returns 0 on success
*/
t_status	dup_all(t_command *cmd)
{
	setup_child_signals();
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, 0) == -1)
			return (1);
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, 1) == -1)
			return (1);
	}
	return (0);
}

/*
Executes a pipeline recursively.
*/
void	execute(t_command *cmd, t_vars *vars)
{
	pid_t		child_pid;
	int			stat;

	if (!cmd)
		return ;
	child_pid = fork();
	if (!child_pid)
	{
		if (dup_all(cmd))
			exit(1);
		if (is_builtin(cmd->command))
			clean_exit(execute_builtin(cmd, vars), vars);
		else
			find_and_exec(cmd, vars);
	}
	else
	{
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		execute(cmd->next, vars);
		waitpid(child_pid, &stat, 0);
		if (!(cmd->next))
			*(vars->status) = exit_status(stat);
	}
}
