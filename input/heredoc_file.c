/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:12:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 19:35:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_heredoc_file(char **path, t_vars *vars)
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

static int	write_heredoc_lines(int fd, char **path,
		t_vars *vars, const char *delimiter)
{
	char	*line;
	char	*expanded;
	int		is_delimiter;

	while (1)
	{
		line = read_shell_line(vars, "heredoc> ");
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		is_delimiter = line_is_delimiter(line, delimiter);
		expanded = NULL;
		if (!is_delimiter)
			expanded = expand_heredoc_line(line, vars);
		if (vars->history_entry)
			vars->history_entry = append_line(vars->history_entry, line);
		else
			free(line);
		if (is_delimiter)
		{
			break ;
		}
		if (!expanded || write(fd, expanded, ft_strlen(expanded)) < 0
			|| write(fd, "\n", 1) < 0)
		{
			free(expanded);
			close(fd);
			free(*path);
			return (0);
		}
		free(expanded);
	}
	return (1);
}

char	*create_heredoc_file(t_vars *vars, const char *delimiter)
{
	char	*path;
	int		fd;

	fd = open_heredoc_file(&path, vars);
	if (fd < 0)
		return (NULL);
	if (!write_heredoc_lines(fd, &path, vars, delimiter))
		return (NULL);
	close(fd);
	return (path);
}
