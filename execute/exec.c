/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:07:13 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/06 18:11:09 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_status = 0;

/*
Executes a built-in directly in the current process.
In a pipeline, this will not be the root process.
In a single command, it is.
*/
void	execute_builtin(t_command *cmd, t_vars *vars)
{
	if (!ft_strcmp(cmd->command, "echo"))
		ft_echo(cmd, vars);
	else if (!ft_strcmp(cmd->command, "cd"))
		ft_cd(cmd, vars);
	else if (!ft_strcmp(cmd->command, "pwd"))
		ft_pwd(cmd);
	else if (!ft_strcmp(cmd->command, "export"))
		ft_export(cmd, vars);
	else if (!ft_strcmp(cmd->command, "unset"))
		ft_unset(cmd, vars);
	else if (!ft_strcmp(cmd->command, "env"))
		ft_env(cmd, vars);
	else if (!ft_strcmp(cmd->command, "exit"))
		ft_exit(cmd);
}

/*
For single command. dups all file descriptors
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

	if (is_builtin(cmd->command))
	{
		execute_builtin(cmd, vars);
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
		waitpid(child_pid, NULL, 0);
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

	if (!cmd)
		return ;
	child_pid = fork();
	if (!child_pid) // in the child
	{
		if (dup_all(cmd))
			exit(1);
		if (is_builtin(cmd->command))
			execute_builtin(cmd, vars);
		else
			find_and_exec(cmd, vars);
		exit(0);
	}
	else
	{
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		execute(cmd->next, vars);
		waitpid(child_pid, NULL, 0);
	}
}