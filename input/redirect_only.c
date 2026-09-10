/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_only.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:25:00 by saecker          #+#    #+#             */
/*   Updated: 2026/09/09 19:25:00 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_command	*echo_command(void)
{
	char	**av;

	av = malloc(sizeof(char *) * 3);
	if (!av)
		return (NULL);
	av[0] = ft_strdup("echo");
	av[1] = ft_strdup("-n");
	av[2] = NULL;
	if (!av[0] || !av[1])
	{
		free_arr((void **)av);
		return (NULL);
	}
	return (new_command(av, 2, 0));
}

void	append_redirect_only(struct s_redir *rd)
{
	t_command	*echo;
	t_command	*target;
	char		*name;
	int			i;

	echo = echo_command();
	if (!echo)
		return ;
	append_command(rd->head, rd->tail, echo);
	i = 0;
	while (i + 1 < rd->ac && is_redirect_token(rd->av[i]))
	{
		name = rd->av[i + 1];
		if (!ft_strcmp(rd->av[i], ">|"))
			rd->av[i][1] = '\0';
		if (!set_command_limiter(*rd->tail, rd->av[i]))
			return ;
		target = target_command(name);
		if (!target)
			return ;
		append_command(rd->head, rd->tail, target);
		i += 2;
	}
	if (rd->has_pipe)
		set_command_limiter(*rd->tail, "|");
}
