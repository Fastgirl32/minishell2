/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:49:41 by lstarek           #+#    #+#             */
/*   Updated: 2026/08/31 15:54:25 by saecker          ###   ########.fr       */
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
	setup_parent_signals();
	print_banner();
	while (vars->stop == 0)
	{
		input_process(vars);
	}
	free_vars(vars);
	ft_putstr_fd("exit\n", 2);
	return (status);
}
