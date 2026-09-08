/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:24:48 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/08 08:33:37 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Removes the internal cat argument after heredoc setup. */
static void	clear_internal_cat(t_command *cmd)
{
	free(cmd->argv[1]);
	cmd->argv[1] = NULL;
	cmd->ac = 1;
}

/* Connects a command to its prepared heredoc pipe. */
int	setup_heredoc(t_command *cmd)
{
	int		pipe_fd[2];
	char	*limiter;

	if (!cmd || !cmd->limiter)
		return (0);
	limiter = cmd->limiter;
	if (!ft_strcmp(limiter, "|") && !ft_strcmp(cmd->command, "cat")
		&& cmd->ac == 2 && cmd->argv && cmd->argv[1])
		limiter = cmd->argv[1];
	else if (!ft_strcmp(limiter, "|") || is_redirect_op(limiter))
		return (0);
	if (pipe(pipe_fd) != 0)
		return (1);
	if (!fill_heredoc_pipe(pipe_fd[1], limiter))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (limiter != cmd->limiter)
		clear_internal_cat(cmd);
	close(pipe_fd[1]);
	replace_cmd_fd_in(cmd, pipe_fd[0]);
	return (0);
}

/* Prepares every heredoc in a command list. */
int	prepare_heredocs(t_command *head)
{
	while (head)
	{
		if (setup_heredoc(head) != 0)
			return (1);
		head = head->next;
	}
	return (0);
}

/* Frees collected heredoc lines after an allocation failure. */
void	free_lines_partial(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

/* Grows the heredoc line array and appends one line. */
char	**grow_lines(char **lines, int count, char *line)
{
	char	**new_lines;
	int		i;

	new_lines = malloc(sizeof(char *) * (size_t)(count + 2));
	if (!new_lines)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_lines[i] = lines[i];
		i++;
	}
	new_lines[count] = line;
	new_lines[count + 1] = NULL;
	free(lines);
	return (new_lines);
}
/* Input heredoc helpers. */
