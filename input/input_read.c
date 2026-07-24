#include "minishell.h"
#include <readline/readline.h>

char	*read_line_prompt(t_vars *vars, const char *prompt)
{
	char	*line;

	(void)vars;
	line = readline(prompt);
	return (line);
}

char	*read_line_plain(void)
{
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*read_shell_line(t_vars *vars, const char *prompt)
{
	if (isatty(STDIN_FILENO))
		return (read_line_prompt(vars, prompt));
	return (read_line_plain());
}

char	*append_line(char *line, char *more)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(line, "\n");
	free(line);
	if (!tmp)
		return (free(more), NULL);
	joined = ft_strjoin(tmp, more);
	free(tmp);
	free(more);
	return (joined);
}

const char	*quote_prompt(char q)
{
	if (q == '"')
		return ("dquote> ");
	return ("quote> ");
}
