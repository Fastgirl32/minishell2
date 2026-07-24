#include "minishell.h"

int	setup_heredoc(t_command *cmd)
{
	int	pipe_fd[2];

	if (!cmd || !cmd->limiter || !ft_strcmp(cmd->limiter, "|")
		|| is_redirect_op(cmd->limiter))
		return (0);
	if (pipe(pipe_fd) != 0)
		return (1);
	if (!fill_heredoc_pipe(pipe_fd[1], cmd->limiter))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	close(pipe_fd[1]);
	replace_cmd_fd_in(cmd, pipe_fd[0]);
	return (0);
}

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

char	*normalize_heredoc_line(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}
