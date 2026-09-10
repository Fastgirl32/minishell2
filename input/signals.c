/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 01:47:07 by saecker           #+#    #+#             */
/*   Updated: 2026/09/10 02:06:51 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ignore_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

void	set_heredoc_signal_mode(int active)
{
	if (active)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else
		setup_parent_signals();
}

int	take_interactive_sigint(void)
{
	if (g_interrupted == 0)
		return (0);
	g_interrupted = 0;
	return (1);
}

int	take_heredoc_sigint(void)
{
	if (g_interrupted == 0)
		return (0);
	g_interrupted = 0;
	return (1);
}

void	restore_parent_sigint(void)
{
	signal(SIGINT, sigint_handler);
}
