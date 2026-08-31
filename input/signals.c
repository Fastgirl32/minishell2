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
	rl_done = 1;
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	take_interactive_sigint(void)
{
	if (!g_interrupted)
		return (0);
	g_interrupted = 0;
	return (1);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
