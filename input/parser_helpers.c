/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:35:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/07 16:35:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Skips spaces and tabs starting at an input position. */
size_t	skip_blanks(const char *line, size_t i)
{
	while (line[i] && is_blank(line[i]))
		i++;
	return (i);
}

/* Finds the start of the segment after a pipe. */
size_t	next_segment_start(const char *line, size_t pos)
{
	while (line[pos] && is_blank(line[pos]))
		pos++;
	if (line[pos] == '|')
		pos++;
	return (pos);
}

/* Copies command arguments while removing redirection pairs. */
char	**copy_command_args(char **av, int ac, int *out_ac)
{
	char	**args;
	int		i;
	int		j;

	args = malloc(sizeof(char *) * (size_t)(ac + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ac)
	{
		if (is_redirect_op(av[i]))
			i += 2;
		else
		{
			args[j] = ft_strdup(av[i]);
			if (!args[j])
				return (args[j] = NULL, free_arr((void **)args), NULL);
			j++;
			i++;
		}
	}
	args[j] = NULL;
	*out_ac = j;
	return (args);
}

/* Creates a command containing one argument. */
t_command	*new_single_arg_command(char *arg)
{
	char		**av;
	t_command	*cmd;

	av = malloc(sizeof(char *) * 2);
	if (!av)
		return (NULL);
	av[0] = ft_strdup(arg);
	if (!av[0])
		return (free(av), NULL);
	av[1] = NULL;
	cmd = new_command(av, 1, 0);
	if (!cmd)
		free_arr((void **)av);
	return (cmd);
}

/* Adds the internal cat command used for a piped heredoc. */
int	append_cat_heredoc(struct s_redir *rd)
{
	char		**args;
	t_command	*cmd;

	args = malloc(sizeof(char *) * 3);
	if (!args)
		return (0);
	args[0] = ft_strdup("cat");
	args[1] = ft_strdup(rd->av[rd->op_i + 1]);
	args[2] = NULL;
	if (!args[0] || !args[1])
		return (free_arr((void **)args), 0);
	cmd = new_command(args, 2, 0);
	if (!cmd)
		return (free_arr((void **)args), 0);
	if (!set_command_limiter(cmd, "|"))
		return (free_list(cmd), 0);
	append_command(rd->head, rd->tail, cmd);
	return (1);
}
