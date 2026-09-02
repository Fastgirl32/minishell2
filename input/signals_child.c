/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:10:00 by saecker           #+#    #+#             */
/*   Updated: 2026/09/02 12:58:19 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
While a foreground child is running, the parent must not react to
SIGINT itself (the child already gets it via the shared terminal
process group). Otherwise the parent's handler would redisplay the
prompt while the child is still busy, printing it twice.
*/
void	ignore_parent_sigint(void)
{
	signal(SIGINT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
