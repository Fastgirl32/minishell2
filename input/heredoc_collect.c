#include "minishell.h"

int	collect_heredoc_line(char ***lines, int *count, const char *limiter)
{
	char	*line;
	char	**new_lines;

	line = read_heredoc_line();
	if (!line)
		return (0);
	if (delimiter_found(line, limiter))
		return (free(line), 0);
	line = normalize_heredoc_line(line);
	new_lines = grow_lines(*lines, *count, line);
	if (!new_lines)
		return (free(line), -1);
	*lines = new_lines;
	(*count)++;
	return (1);
}

char	**collect_heredoc_lines(const char *limiter, int *out_count)
{
	char	**lines;
	int		count;
	int		res;

	lines = malloc(sizeof(char *));
	if (!lines)
		return (NULL);
	lines[0] = NULL;
	count = 0;
	res = 1;
	while (res > 0)
		res = collect_heredoc_line(&lines, &count, limiter);
	if (res < 0)
		return (free_lines_partial(lines, count), NULL);
	*out_count = count;
	return (lines);
}

t_command	*alloc_heredoc_cmd(char **lines, int count)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (free_arr((void **)lines), NULL);
	cmd->command = ft_strdup("");
	if (!cmd->command)
		return (free_arr((void **)lines), free(cmd), NULL);
	cmd->limiter = NULL;
	cmd->ac = count;
	cmd->argv = lines;
	cmd->next = NULL;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	return (cmd);
}

t_command	*build_heredoc(const char *limiter)
{
	char	**lines;
	int		count;

	count = 0;
	lines = collect_heredoc_lines(limiter, &count);
	if (!lines)
		return (NULL);
	return (alloc_heredoc_cmd(lines, count));
}
