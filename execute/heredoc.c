/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:01:19 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/10 14:01:21 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Returns 1 if temporary heredoc file should be read.
Returns 0 otherwise.
*/
_Bool	is_heredoc(t_command *cmd)
{
	return (ft_strncmp("/tmp/minishell-heredoc-", cmd->command, 23) == 0);
}

/*
Redirects the contents of the temporary heredoc file to stdout.
On success, returns 0.
On fail, returns 1.
*/
t_status	print_heredoc(t_command *cmd)
{
	char	*line;
	int		out;
	int		heredoc;

	if (!is_heredoc(cmd))
		return (1);
	out = STDOUT_FILENO;
	if (cmd->is_single)
		out = cmd->fd_out;
	heredoc = open(cmd->command, O_RDONLY);
	if (heredoc < 0)
		return (1);
	line = get_next_line(heredoc);
	while (line)
	{
		ft_putstr_fd(line, out);
		free(line);
		line = get_next_line(heredoc);
	}
	close(heredoc);
	return (0);
}
