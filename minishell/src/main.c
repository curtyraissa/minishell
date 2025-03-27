#include "minishell.h"

// Global variable to store the received signal number (SIGINT, SIGQUIT, etc.)
int	g_signal = 0;

int	main(int argc, char **argv, char *envp[])
{
	t_shell	*shell;

	// Initializes the shell structure with argv and envp
	shell = init_struct(argv, envp);
	if (!shell)
		return (1);

	// Finds and stores the index of the HOME variable in envp
	shell->home_index = var_search(envp, "HOME");

	// If extra arguments are passed, exit silently
	if (argc != 1 || argv[1])
		return (0);

	// Set up signal handlers (e.g., for Ctrl-C, Ctrl-\)
	signals();

	// Starts the shell loop
	init_minishell(shell);

	return (0);
}
