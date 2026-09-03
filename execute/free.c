/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:35:19 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/01 17:35:21 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Frees a double pointer passed as (void **)
*/
void	free_arr(void **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
Frees the vars struct properly.
*/
void	free_vars(t_vars *vars)
{
	if (!vars)
		return ;
	free_arr((void **)(vars->history));
	free_arr((void **)(vars->env));
	free(vars);
}

void	clean_exit(t_status status, t_vars *vars)
{
	free_list(vars->list);
	free_vars(vars);
	exit(status);
}

/*
frees a t_command linked list.
*/
void	free_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		ft_close(&cmd->fd_in);
		ft_close(&cmd->fd_out);
		free(cmd->command);
		free(cmd->limiter);
		free_arr((void **)cmd->argv);
		free(cmd);
		cmd = next;
	}
}
