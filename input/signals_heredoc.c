/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 07:54:27 by saecker           #+#    #+#             */
/*   Updated: 2026/09/11 08:29:40 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
