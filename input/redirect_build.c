/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_build.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:10:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 19:10:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	append_redirect_target(struct s_redir *rd, int i)
{
	t_command	*target;
	char		*name;

	name = rd->av[i + 1];
	if (!ft_strcmp(rd->av[i], ">|"))
		rd->av[i][1] = '\0';
	if (is_heredoc_token(rd->av[i]))
		name = create_heredoc_file(rd->vars, rd->av[i + 1]);
	if (!name)
		return (0);
	if (!set_command_limiter(*rd->tail, rd->av[i]))
	{
		if (name != rd->av[i + 1])
			free(name);
		return (0);
	}
	target = target_command(name);
	if (name != rd->av[i + 1])
		free(name);
	if (!target)
		return (0);
	append_command(rd->head, rd->tail, target);
	return (1);
}

static int	append_redirect_targets(struct s_redir *rd)
{
	int	i;

	i = 0;
	while (i < rd->ac)
	{
		if (is_redirect_token(rd->av[i]))
		{
			if (!append_redirect_target(rd, i))
				return (0);
			i += 2;
		}
		else
			i++;
	}
	return (1);
}

static int	prepare_heredoc(struct s_redir *rd, int i)
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

static void	append_regular_redirect(struct s_redir *rd, char **args, int count)
{
	t_command	*cmd;

	cmd = new_command(args, count, 0);
	if (!cmd)
		return ;
	append_command(rd->head, rd->tail, cmd);
	if (!append_redirect_targets(rd))
		return ;
	if (rd->has_pipe)
		set_command_limiter(*rd->tail, "|");
}

void append_redirect_segment(struct s_redir *rd)
{
	char	**args;
	int		count;
	int		i;

	i = 0;
	while (i + 1 < rd->ac)
	{
		if (is_heredoc_token(rd->av[i]))
		{
			if (!prepare_heredoc(rd, i))
				return ;
			i++;
		}
		else
			i++;
	}
	if (!ft_strcmp(rd->av[0], ">") || !ft_strcmp(rd->av[0], ">>"))
	{
		append_redirect_only(rd);
		return ;
	}
	args = collect_command_args(rd->av, rd->ac, &count);
	if (!args)
		return ;
	if (count == 0)
	{
		free_arr((void **)args);
		append_redirect_only(rd);
		return ;
	}
	append_regular_redirect(rd, args, count);
}