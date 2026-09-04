/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:40 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/04 12:46:33 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This is the only global.
static volatile sig_atomic_t	g_interrupted;

static void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupted = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/*
While a foreground child is running, the parent must not react to
SIGINT itself (the child already gets it via the shared terminal
process group). Otherwise the parent's handler would redisplay the
prompt while the child is still busy, printing it twice.
*/
void	restore_parent_sigint(void)
{
	signal(SIGINT, sigint_handler);
}

int	take_interactive_sigint(void)
{
	if (!g_interrupted)
		return (0);
	g_interrupted = 0;
	return (1);
}
