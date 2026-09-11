/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/11 07:38:51 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
ENTRY POINT
*/
int	main(int ac, char **av, char **env)
{
	t_vars	*vars;
	int		status;

	(void)ac;
	(void)av;
	status = 0;
	vars = init_vars(env, &status);
	if (!vars)
		return (1);
	setup_parent_signals();
	print_banner();
	while (vars->stop == 0)
	{
		input_process(vars);
	}
	ft_printf("exit\n");
	free_vars(vars);
	return (status);
}
