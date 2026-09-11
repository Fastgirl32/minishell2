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

static int	write_heredoc_line(int fd, char **path, t_vars *vars, char *line)
{
	char	*expanded;

	expanded = expand_heredoc_line(line, vars);
	if (vars->history_entry)
		vars->history_entry = append_line(vars->history_entry, line);
	else
		free(line);
	if (!expanded || write(fd, expanded, ft_strlen(expanded)) < 0 || write(fd,
			"\n", 1) < 0)
	{
		free(expanded);
		heredoc_write_error(fd, path);
		return (-1);
	}
	free(expanded);
	return (0);
}

static int	write_heredoc_lines(int fd, char **path, t_vars *vars,
		const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = read_shell_line(vars, "heredoc> ");
		if (here_doc_received_sigint(fd, path) == -1)
			return (-1);
		if (!line)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line_is_delimiter(line, delimiter))
		{
			if (vars->history_entry)
				vars->history_entry = append_line(vars->history_entry, line);
			else
				free(line);
			break ;
		}
		if (write_heredoc_line(fd, path, vars, line) == -1)
			return (0);
	}
	return (1);
}

char	*create_heredoc_file(t_vars *vars, const char *delimiter)
{
	char	*path;
	int		fd;
	int		ret;

	set_heredoc_signal_mode(1);
	fd = open_heredoc_file(&path, vars);
	if (fd < 0)
	{
		set_heredoc_signal_mode(0);
		return (NULL);
	}
	ret = write_heredoc_lines(fd, &path, vars, delimiter);
	set_heredoc_signal_mode(0);
	if (ret <= 0)
		return (NULL);
	close(fd);
	return (path);
}

int	prepare_heredoc(struct s_redir *rd, int i)
{
	char	*name;
	int		j;

	name = create_heredoc_file(rd->vars, rd->av[i + 1]);
	if (!name)
		return (0);
	free(rd->av[i]);
	free(rd->av[i + 1]);
	rd->av[i] = name;
	j = i + 1;
	while (j < rd->ac - 1)
	{
		rd->av[j] = rd->av[j + 1];
		j++;
	}
	rd->av[rd->ac - 1] = NULL;
	rd->ac--;
	return (1);
}
