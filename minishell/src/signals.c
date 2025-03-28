#include "minishell.h"

// Handles SIGINT signal during interactive prompt input
void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrlc = SIGINT;
		printf("\n");
		rl_replace_line("", 0);
		if (rl_line_buffer[0] == '\0')
		{
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

// Sets up signal handling for the main shell process
void	signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

// Handles SIGINT inside a heredoc process and exits gracefully
void	heredoc_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrlc = SIGINT;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		exit(SIGINT);
	}
}

// Handles SIGINT inside child processes by setting the global variable
void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
		g_ctrlc = SIGINT;
}
