#include "minishell.h"

int	delimiter_found(const char *line, const char *delimiter)
{
	size_t	line_len;

	if (!line || !delimiter)
		return (0);
	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line_len--;
	if (ft_strlen(delimiter) != line_len)
		return (0);
	return (!ft_strncmp(line, delimiter, line_len));
}

char	*read_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (read_line_prompt(NULL, "heredoc> "));
	return (read_line_plain());
}

int	write_heredoc_line(int fd, const char *line)
{
	if (write(fd, line, ft_strlen(line)) < 0)
		return (0);
	if (isatty(STDIN_FILENO) && write(fd, "\n", 1) < 0)
		return (0);
	return (1);
}

void	replace_cmd_fd_in(t_command *cmd, int new_fd)
{
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	cmd->fd_in = new_fd;
}

int	fill_heredoc_pipe(int fd, const char *limiter)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			return (1);
		if (delimiter_found(line, limiter))
			return (free(line), 1);
		if (!write_heredoc_line(fd, line))
		{
			free(line);
			return (0);
		}
		free(line);
	}
}
