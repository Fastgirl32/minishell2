#include "minishell.h"
#include <readline/history.h>

char	*trimmed_history_entry(const char *line)
{
	char	*entry;
	int		len;

	len = (int)ft_strlen(line);
	while (len > 0 && line[len - 1] == '\n')
		len--;
	entry = malloc((size_t)len + 1);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, line, (size_t)len + 1);
	return (entry);
}

int	history_reserve(t_vars *vars)
{
	char	**new_history;
	int		new_cap;
	int		i;

	if (vars->history_count + 1 < vars->history_cap)
		return (1);
	new_cap = 32;
	if (vars->history_cap > 0)
		new_cap = vars->history_cap * 2;
	new_history = malloc(sizeof(char *) * (size_t)new_cap);
	if (!new_history)
		return (0);
	i = -1;
	while (++i < vars->history_count)
		new_history[i] = vars->history[i];
	free(vars->history);
	vars->history = new_history;
	vars->history_cap = new_cap;
	return (1);
}

void	history_add(t_vars *vars, const char *line)
{
	char	*entry;

	if (!vars || !line || is_blank_line(line))
		return ;
	entry = trimmed_history_entry(line);
	if (!entry)
		return ;
	if (!entry[0])
	{
		free(entry);
		return ;
	}
	if (!history_reserve(vars))
		return (free(entry));
	vars->history[vars->history_count++] = entry;
	vars->history[vars->history_count] = NULL;
	add_history(entry);
}

void	history_print(t_vars *vars)
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
