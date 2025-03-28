#include "minishell.h"

// Executes a command in a child process and exits with the correct status
void	fork_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_status;

	run_cmd(cmd, shell);
	delete_heredocs(shell, 0, shell->head);
	free_cmd(shell->head);
	exit_status = shell->exit_status;
	free_shell(shell, 2);
	exit(exit_status);
}

// Forks the left side of a pipe, redirecting STDOUT to the pipe
int	fork_function1(t_cmd *pipecmd, t_shell *shell)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGINT, child_signal_handler);
		signal(SIGQUIT, SIG_DFL);
		close(pipecmd->pipefd[0]);
		if (dup2(pipecmd->pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 error");
			exit(-1);
		}
		close(pipecmd->pipefd[1]);
		fork_exit(pipecmd->left, shell);
	}
	return (pid);
}

// Forks the right side of a pipe, redirecting STDIN from the pipe
int	fork_function2(t_cmd *pipecmd, t_shell *shell)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGINT, child_signal_handler);
		signal(SIGQUIT, SIG_DFL);
		close(pipecmd->pipefd[1]);
		if (dup2(pipecmd->pipefd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 error");
			exit(-1);
		}
		close(pipecmd->pipefd[0]);
		fork_exit(pipecmd->right, shell);
	}
	return (pid);
}
