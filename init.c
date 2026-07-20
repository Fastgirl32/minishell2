#include "minishell.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <unistd.h>

static int			is_redirect_op(const char *s);
static void			extract_redirections(char **av, int *ac, char **limiter,
						int *redirect_start);
static int			prepare_heredocs(t_command *head);

int				g_status = 0;

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_blank_line(const char *s)
{
	while (*s)
	{
		if (!is_blank(*s) && *s != '\n')
			return (0);
		s++;
	}
	return (1);
}

static int	is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

static int	redir_op_len(const char *line, size_t i, char quote)
{
	if (quote)
		return (0);
	if (line[i] == '<' || line[i] == '>')
	{
		if (line[i + 1] == line[i])
			return (2);
		return (1);
	}
	return (0);
}

static int	append_char(char **str, size_t *len, size_t *cap, char c)
{
	char	*new_str;
	size_t	new_cap;
	size_t	i;

	if (*len + 2 <= *cap)
	{
		(*str)[(*len)++] = c;
		(*str)[*len] = '\0';
		return (1);
	}
	new_cap = *cap;
	if (new_cap == 0)
		new_cap = 32;
	while (*len + 2 > new_cap)
		new_cap *= 2;
	new_str = malloc(new_cap);
	if (!new_str)
		return (0);
	i = 0;
	while (i < *len)
	{
		new_str[i] = (*str)[i];
		i++;
	}
	new_str[*len] = '\0';
	free(*str);
	*str = new_str;
	*cap = new_cap;
	(*str)[(*len)++] = c;
	(*str)[*len] = '\0';
	return (1);
}

static int	append_str(char **str, size_t *len, size_t *cap, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		if (!append_char(str, len, cap, src[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	*check_env(char **env, const char *name)
{
	int		i;
	size_t	name_len;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(name, env[i], name_len) && env[i][name_len] == '=')
			return (value(env[i]));
		i++;
	}
	return (NULL);
}

static char	*copy_token(const char *line, size_t start, size_t end, char **env)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*token;
	char	*val;
	char	*var;
	char	*status_str;
	char	*dyn;
	size_t	len;
	size_t	cap;

	dyn = malloc(32);
	if (!dyn)
		return (NULL);
	cap = 32;
	len = 0;
	dyn[0] = '\0';
	i = start;
	quote = 0;
	while (i < end)
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
		{
			quote = line[i];
			i++;
			continue ;
		}
		if (quote && line[i] == quote)
		{
			quote = 0;
			i++;
			continue ;
		}
		if (line[i] == '$' && quote != '\'')
		{
			if ((i + 1) < end && line[i + 1] == '?')
			{
				status_str = ft_itoa(g_status);
				if (!status_str)
				{
					free(dyn);
					return (NULL);
				}
				if (!append_str(&dyn, &len, &cap, status_str))
				{
					free(status_str);
					free(dyn);
					return (NULL);
				}
				free(status_str);
				i += 2;
				continue ;
			}
			j = i + 1;
			while (j < end && is_var_char(line[j]))
				j++;
			if (j > i + 1)
			{
				var = ft_substr(line, (unsigned int)(i + 1), j - (i + 1));
				if (!var)
				{
					free(dyn);
					return (NULL);
				}
				val = check_env(env, var);
				if (val && !append_str(&dyn, &len, &cap, val))
				{
					free(var);
					free(val);
					free(dyn);
					return (NULL);
				}
				free(var);
				free(val);
				i = j;
				continue ;
			}
		}
		if (!append_char(&dyn, &len, &cap, line[i]))
		{
			free(dyn);
			return (NULL);
		}
		i++;
	}
	token = ft_strdup(dyn);
	free(dyn);
	return (token);
}

static char	*read_line_prompt(t_vars *vars, const char *prompt)
{
	char	*line;

	(void)vars;
	line = readline(prompt);
	return (line);
}

static char	*read_line_plain(void)
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

static int	delimiter_found(const char *line, const char *delimiter)
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

static int	setup_heredoc(t_command *cmd)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd || !cmd->limiter)
		return (0);
	if (pipe(pipe_fd) != 0)
		return (1);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = read_line_prompt(NULL, "heredoc> ");
		else
			line = read_line_plain();
		if (!line)
			break ;
		if (delimiter_found(line, cmd->limiter))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		if (isatty(STDIN_FILENO))
			write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	cmd->fd_in = pipe_fd[0];
	return (0);
}

static void	heredoc_alone(const char *limiter)
{
	char	*line;

	if (!limiter)
		return ;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = read_line_prompt(NULL, "heredoc> ");
		else
			line = read_line_plain();
		if (!line)
			break ;
		if (delimiter_found(line, limiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
}

static int	contains_redirects(char **av, int ac)
{
	int	i;

	i = 0;
	while (i < ac)
	{
		if (!is_redirect_op(av[i]))
			return (0);
		if (i + 1 >= ac)
			return (0);
		i += 2;
	}
	return (ac > 0);
}

static void	handle_redirects(char **av, int ac)
{
	int	i;

	i = 0;
	while (i + 1 < ac)
	{
		if (!ft_strcmp(av[i], "<<"))
			heredoc_alone(av[i + 1]);
		i += 2;
	}
}

static int	prepare_heredocs(t_command *head)
{
	while (head)
	{
		if (setup_heredoc(head) != 0)
			return (1);
		head = head->next;
	}
	return (0);
}

static char	**collect_heredoc_lines(const char *limiter, int *out_count)
{
	char	**lines;
	char	**new_lines;
	char	*line;
	int		count;
	int		i;

	lines = malloc(sizeof(char *));
	if (!lines)
		return (NULL);
	lines[0] = NULL;
	count = 0;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = read_line_prompt(NULL, "heredoc> ");
		else
			line = read_line_plain();
		if (!line)
			break ;
		if (delimiter_found(line, limiter))
		{
			free(line);
			break ;
		}
		new_lines = malloc(sizeof(char *) * (size_t)(count + 2));
		if (!new_lines)
		{
			free(line);
			i = 0;
			while (i < count)
				free(lines[i++]);
			free(lines);
			return (NULL);
		}
		i = 0;
		while (i < count)
		{
			new_lines[i] = lines[i];
			i++;
		}
		new_lines[count++] = line;
		new_lines[count] = NULL;
		free(lines);
		lines = new_lines;
	}
	*out_count = count;
	return (lines);
}

static t_command	*build_heredoc(const char *limiter)
{
	t_command	*cmd;
	char		**lines;
	int			count;

	count = 0;
	lines = collect_heredoc_lines(limiter, &count);
	if (!lines)
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		free_arr((void **)lines);
		return (NULL);
	}
	cmd->command = ft_strdup("");
	cmd->limiter = ft_strdup("<<");
	if (!cmd->command || !cmd->limiter)
	{
		free(cmd->command);
		free(cmd->limiter);
		free_arr((void **)lines);
		free(cmd);
		return (NULL);
	}
	cmd->ac = count;
	cmd->argv = lines;
	cmd->next = NULL;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	return (cmd);
}

static void	history_add(t_vars *vars, const char *line)
{
	char	*entry;
	char	**new_history;
	int		len;
	int		i;
	int		new_cap;

	if (!vars || !line || is_blank_line(line))
		return ;
	len = (int)ft_strlen(line);
	while (len > 0 && line[len - 1] == '\n')
		len--;
	entry = malloc((size_t)len + 1);
	if (!entry)
		return ;
	i = 0;
	while (i < len)
	{
		entry[i] = line[i];
		i++;
	}
	entry[i] = '\0';
	if (!entry[0])
	{
		free(entry);
		return ;
	}
	if (vars->history_count + 1 >= vars->history_cap)
	{
		new_cap = 32;
		if (vars->history_cap > 0)
			new_cap = vars->history_cap * 2;
		new_history = malloc(sizeof(char *) * (size_t)new_cap);
		if (!new_history)
		{
			free(entry);
			return ;
		}
		i = 0;
		while (i < vars->history_count)
		{
			new_history[i] = vars->history[i];
			i++;
		}
		free(vars->history);
		vars->history = new_history;
		vars->history_cap = new_cap;
	}
	vars->history[vars->history_count++] = entry;
	vars->history[vars->history_count] = NULL;
	add_history(entry);
}

static void	history_print(t_vars *vars)
{
	int	i;

	if (!vars)
		return ;
	i = 0;
	while (i < vars->history_count)
	{
		ft_printf("%d %s\n", i + 1, vars->history[i]);
		i++;
	}
}

static size_t	segment_end(const char *line, size_t start)
{
	size_t	i;
	char	quote;

	i = start;
	quote = 0;
	while (line[i] && line[i] != '\n')
	{
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		else if (quote && line[i] == quote)
			quote = 0;
		else if (!quote && line[i] == '|')
			break ;
		i++;
	}
	while (i > start && is_blank(line[i - 1]))
		i--;
	return (i);
}

static size_t	count_tokens(const char *line, size_t start, size_t end)
{
	size_t	i;
	size_t	count;
	char	quote;
	int		op_len;

	i = start;
	count = 0;
	while (i < end)
	{
		while (i < end && is_blank(line[i]))
			i++;
		if (i >= end)
			break ;
		op_len = redir_op_len(line, i, 0);
		if (op_len)
		{
			count++;
			i += (size_t)op_len;
			continue ;
		}
		count++;
		quote = 0;
		while (i < end)
		{
			if (!quote && (line[i] == '\'' || line[i] == '"'))
				quote = line[i];
			else if (quote && line[i] == quote)
				quote = 0;
			else if (!quote && (is_blank(line[i]) || redir_op_len(line, i,
						quote)))
				break ;
			i++;
		}
	}
	return (count);
}

static void	free_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->fd_in > 2)
			close(cmd->fd_in);
		if (cmd->fd_out > 2)
			close(cmd->fd_out);
		free(cmd->command);
		free(cmd->limiter);
		free_arr((void **)cmd->argv);
		free(cmd);
		cmd = next;
	}
}

static t_command	*new_command(char **av, int ac)
{
	t_command	*cmd;
	char		*limiter;
	int			redirect_start;

	redirect_start = 0;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	limiter = NULL;
	extract_redirections(av, &ac, &limiter, &redirect_start);
	if (redirect_start || ac < 1 || !av[0])
	{
		free(cmd);
		free(limiter);
		return (NULL);
	}
	cmd->command = ft_strdup(av[0]);
	if (!cmd->command)
	{
		free(cmd);
		free(limiter);
		return (NULL);
	}
	cmd->limiter = limiter;
	cmd->ac = ac;
	if (!ft_strcmp(av[0], "exit") && ac == 1)
		cmd->ac = 0;
	cmd->argv = av;
	cmd->next = NULL;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	return (cmd);
}

static char	**split_tokens(const char *line, size_t start, size_t end, int *ac,
		char **env)
{
	size_t	i;
	size_t	j;
	size_t	word_start;
	char	**av;
	char	*token;
	int		op_len;
	char	quote;

	av = malloc(sizeof(char *) * (count_tokens(line, start, end) + 1));
	if (!av)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		while (i < end && is_blank(line[i]))
			i++;
		if (i >= end)
			break ;
		op_len = redir_op_len(line, i, 0);
		if (op_len)
		{
			token = ft_substr(line, (unsigned int)i, (size_t)op_len);
			if (!token)
			{
				while (j > 0)
					free(av[--j]);
				free(av);
				return (NULL);
			}
			av[j++] = token;
			i += (size_t)op_len;
			continue ;
		}
		word_start = i;
		while (i < end)
		{
			if (line[i] == '\'' || line[i] == '"')
			{
				quote = line[i++];
				while (i < end && line[i] != quote)
					i++;
				if (i < end)
					i++;
				continue ;
			}
			if (is_blank(line[i]) || redir_op_len(line, i, 0))
				break ;
			i++;
		}
		token = copy_token(line, word_start, i, env);
		if (!token)
		{
			while (j > 0)
				free(av[--j]);
			free(av);
			return (NULL);
		}
		av[j++] = token;
	}
	av[j] = NULL;
	if (ac)
		*ac = (int)j;
	return (av);
}

static void	append_command(t_command **head, t_command **tail,
		t_command *new_cmd)
{
	if (!new_cmd)
		return ;
	if (!*head)
		*head = new_cmd;
	else
		(*tail)->next = new_cmd;
	*tail = new_cmd;
}

static char	find_unclosed_quote(const char *s)
{
	char	q;

	q = 0;
	while (*s)
	{
		if (!q && (*s == '\'' || *s == '"'))
			q = *s;
		else if (q && *s == q)
			q = 0;
		s++;
	}
	return (q);
}

static void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static int	is_redirect_op(const char *s)
{
	if (!s)
		return (0);
	if (!ft_strcmp(s, "<") || !ft_strcmp(s, ">"))
		return (1);
	if (!ft_strcmp(s, "<<") || !ft_strcmp(s, ">>"))
		return (1);
	return (0);
}

static int	is_heredoc_op(const char *s)
{
	if (!s)
		return (0);
	return (!ft_strcmp(s, "<<"));
}

void	extract_redirections(char **av, int *ac, char **limit,
		int *redirect_start)
{
	int	i;
	int	j;

	if (!av || !ac || *ac < 1)
		return ;
	if (redirect_start)
		*redirect_start = is_redirect_op(av[0]);
	i = 0;
	j = 0;
	while (i < *ac)
	{
		if (is_redirect_op(av[i]))
		{
			if (is_heredoc_op(av[i]) && !*limit && i + 1 < *ac)
				*limit = ft_strdup(av[i + 1]);
			free(av[i]);
			av[i] = NULL;
			if (i + 1 < *ac)
				free(av[i + 1]);
			i += 2;
		}
		else
		{
			av[j++] = av[i++];
		}
	}
	av[j] = NULL;
	*ac = j;
}

t_vars	*init_vars(char **env)
{
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (NULL);
	vars->env = env;
	vars->history = NULL;
	vars->history_count = 0;
	vars->history_cap = 0;
	vars->stop = 0;
	return (vars);
}

void	make_list(t_vars *vars, char *line)
{
	size_t		i;
	size_t		start;
	size_t		end;
	t_command	*head;
	t_command	*tail;
	t_command	*cmd;
	char		**av;
	int			ac;
	pid_t		child_pid;

	head = NULL;
	tail = NULL;
	i = 0;
	while (line[i] && is_blank(line[i]))
		i++;
	if (line[i] == '#')
		return ;
	while (line[i])
	{
		while (line[i] && is_blank(line[i]))
			i++;
		if (!line[i] || line[i] == '\n')
			break ;
		start = i;
		end = segment_end(line, start);
		av = split_tokens(line, start, end, &ac, vars->env);
		if (av)
		{
			if (contains_redirects(av, ac))
			{
				cmd = NULL;
				if (ac >= 2 && !ft_strcmp(av[0], "<<"))
					cmd = build_heredoc(av[1]);
				else
					handle_redirects(av, ac);
				if (cmd)
					append_command(&head, &tail, cmd);
				free_arr((void **)av);
			}
			else
			{
				cmd = new_command(av, ac);
				if (cmd)
					append_command(&head, &tail, cmd);
				else
					free_arr((void **)av);
			}
		}
		i = end;
		if (line[i] == '|')
			i++;
	}
	vars->list = head;
	if (head)
	{
		if (!head->next && head->command && head->command[0] == '\0')
		{
			free_list(head);
			vars->list = NULL;
			return ;
		}
		if (!head->next && !ft_strcmp(head->command, "history"))
		{
			history_print(vars);
			free_list(head);
			vars->list = NULL;
			return ;
		}
		if (!connect_pipes(head) && !prepare_heredocs(head) && !establish_redirects(head))
		{
			if (head->next == NULL)
			{
				if (is_builtin(head->command))
					execute_builtin(head, vars->env);
				else
				{
					child_pid = fork();
					if (!child_pid) //child
						find_and_exec(head,recreate_env(vars->env));
					else //parent waits for child
						waitpid(child_pid, NULL, 0);
				}
			}
			else
			{
				execute(head, vars->env);
			} 
		}
	}
	free_list(head);
	vars->list = NULL;
}

void	get_line(t_vars *vars)
{
	char *line;
	char q;
	char *more;
	const char *prompt;

	line = NULL;
	if (isatty(STDIN_FILENO))
		line = read_line_prompt(vars, "minishell> ");
	else
		line = read_line_plain();
	if (!line)
	{
		if (errno != EINTR)
			vars->stop = 1;
		return ;
	}
	/* if quotes are left open, prompt for continuation lines */
	q = find_unclosed_quote(line);
	while (q)
	{
		if (q == '"')
			prompt = "dquote> ";
		else
			prompt = "quote> ";
		if (isatty(STDIN_FILENO))
			more = read_line_prompt(vars, prompt);
		else
			more = read_line_plain();
		if (!more)
		{
			if (errno != EINTR)
				vars->stop = 1;
			free(line);
			return ;
		}
		/* append '\n' + more to line */
		char *tmp = ft_strjoin(line, "\n");
		char *joined = ft_strjoin(tmp, more);
		free(tmp);
		free(line);
		free(more);
		line = joined;
		q = find_unclosed_quote(line);
	}
	if (vars->stop)
	{
		free(line);
		return ;
	}
	history_add(vars, line);
	make_list(vars, line);
	free(line);
}