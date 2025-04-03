/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:25 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 09:59:47 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a NULL-terminated array of strings
void	free_split(char **split)
{
	int	i;

	i = 0;
	if (split)
	{
		while (split[i])
		{
			free(split[i]);
			i++;
		}
		free(split);
	}
}

/*Handles execution inside the child process: 
executes the command or exits on failure*/
void	handle_child_process(char *path, t_cmd *execcmd, t_shell *shell)
{
	int		flag;
	char	*tmp;

	flag = 0;
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, SIG_DFL);
	execve(path, execcmd->av, shell->env);
	perror("execve error");
	if (access(path, F_OK) == -1)
	{
		tmp = ft_strjoin("/", execcmd->av[0]);
		if (ft_strncmp(path, tmp, ft_strlen(path)) == 0)
			free(path);
		free(tmp);
		flag = 1;
	}
	free_shell(shell, EXIT_CMD);
	if (flag == 1)
		exit(127);
	exit(126);
}

//Handles parent process after forking:waits and sets appropriate exit status
void	handle_parent_process(int pid, char *path,
							t_cmd *execcmd, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		shell->exit_status = 130;
	else if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 1;
	if (path != execcmd->av[0])
		free(path);
}
