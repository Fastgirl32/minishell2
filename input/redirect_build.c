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
	if (is_heredoc_token(rd->av[i]))
		set_command_limiter(*rd->tail, "<");
	else if (!set_command_limiter(*rd->tail, rd->av[i]))
		return (0);
	target = target_command(name);
	if (name != rd->av[i + 1])
		free(name);
	if (target)
		append_command(rd->head, rd->tail, target);
	return (1);
}

static void	append_redirect_targets(struct s_redir *rd)
{
	int	i;

	i = 0;
	while (i + 1 < rd->ac)
	{
		if (!is_redirect_token(rd->av[i]))
			i++;
		else if (!append_redirect_target(rd, i))
			return ;
		i += 2;
	}
}

static void	append_commandless_heredoc(struct s_redir *rd)
{
	char		**args;
	t_command	*cmd;
	t_command	*target;
	char		*name;

	args = malloc(sizeof(char *) * 2);
	if (!args)
		return ;
	args[0] = ft_strdup("cat");
	args[1] = NULL;
	if (!args[0])
		return (free(args));
	cmd = new_command(args, 1, 0);
	if (!cmd)
		return ;
	append_command(rd->head, rd->tail, cmd);
	name = create_heredoc_file(rd->vars, rd->av[1]);
	target = target_command(name);
	free(name);
	if (target && set_command_limiter(*rd->tail, "<"))
		append_command(rd->head, rd->tail, target);
}

static void	append_regular_redirect(struct s_redir *rd,
		char **args, int count)
{
	t_command	*cmd;

	cmd = new_command(args, count, 0);
	if (!cmd)
		return ;
	append_command(rd->head, rd->tail, cmd);
	append_redirect_targets(rd);
	if (rd->has_pipe)
		set_command_limiter(*rd->tail, "|");
}

void	append_redirect_segment(struct s_redir *rd)
{
	char		**args;
	int			count;

	args = collect_command_args(rd->av, rd->ac, &count);
	if (!args)
		return ;
	if (count == 0)
	{
		free_arr((void **)args);
		if (!is_heredoc_token(rd->av[0]) || rd->ac < 2)
		{
			append_redirect_only(rd);
			return ;
		}
		append_commandless_heredoc(rd);
		return ;
	}
	append_regular_redirect(rd, args, count);
}
