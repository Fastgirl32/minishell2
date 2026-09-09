/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:25:40 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/09 23:39:27 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static volatile sig_atomic_t	g_interrupted;

static void	sigint_handler(int sig)
{
	(void)sig;
	if (g_interrupted == 2)
		g_interrupted = 3;
	else
		g_interrupted = 1;
	write(1, "\n", 1);
	rl_done = 1;
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_parent_sigint(void)
{
	signal(SIGINT, sigint_handler);
}

void	set_heredoc_signal_mode(int active)
{
	if (active)
		g_interrupted = 2;
	else if (g_interrupted == 2)
		g_interrupted = 0;
}

int	take_interactive_sigint(void)
{
	int	state;

	state = g_interrupted;
	g_interrupted = 0;
	return (state);
}