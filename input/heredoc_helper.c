/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 07:45:03 by saecker           #+#    #+#             */
/*   Updated: 2026/09/11 07:45:33 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_heredoc_file(char **path, t_vars *vars)
{
	char	*prefix;
	char	*number_text;
	int		fd;

	prefix = ft_strdup("/tmp/minishell-heredoc-");
	if (!prefix)
		return (-1);
	while (1)
	{
		number_text = ft_itoa((int)vars->heredoc_number++);
		if (!number_text)
			return (free(prefix), -1);
		*path = ft_strjoin(prefix, number_text);
		free(number_text);
		if (!*path)
			return (free(prefix), -1);
		fd = open(*path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			return (free(prefix), fd);
		free(*path);
		if (errno != EEXIST)
			return (free(prefix), -1);
	}
}

int	here_doc_received_sigint(int fd, char **path)
{
	if (take_heredoc_sigint())
	{
		close(fd);
		unlink(*path);
		free(*path);
		*path = NULL;
		return (-1);
	}
	return (0);
}

void	heredoc_write_error(int fd, char **path)
{
	close(fd);
	unlink(*path);
	free(*path);
	*path = NULL;
}
