/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:40 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/09 22:08:02 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// This is the only global.
static volatile sig_atomic_t	g_interrupted;

/* Handles SIGINT while the parent shell is waiting for input. */
static void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupted = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* Installs the shell's interactive signal handlers. */
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
/* Restores the default SIGINT behavior for the parent. */
void	restore_parent_sigint(void)
{
	signal(SIGINT, sigint_handler);
}

/* Returns and clears the pending interactive SIGINT state. */
int	take_interactive_sigint(void)
{
	if (g_interrupted == 0)
		return (0);
	g_interrupted = 0;
	return (1);
}
