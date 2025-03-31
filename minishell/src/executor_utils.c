/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:19 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:20 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Combines each directory in PATH with the command name and checks if it's executable
char	*get_cmds_path(char	*path, char	*cmd)
{
	int		i;
	char	*true_cmd;
	char	*full_path;
	char	**directory;

	true_cmd = NULL;
	true_cmd = ft_strjoin("/", cmd);
	directory = ft_split(path, ':');
	if (!directory)
		return (NULL);
	i = -1;
	while (directory[++i])
	{
		full_path = ft_strjoin(directory[i], true_cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(true_cmd);
			free_split(directory);
			return (full_path);
		}
		free(full_path);
	}
	free(true_cmd);
	free_split(directory);
	return (NULL);
}

// Retrieves the full path of a command by searching the PATH variable
char	*get_cmd_path(char **env, char *cmd)
{
	int		i;
	char	*path;
	char	*real_path;

	i = 0;
	path = NULL;
	if (*cmd == '\0')
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp("PATH", env[i], 4) == 0)
		{
			path = ft_strdup(env[i] + 5);
			break ;
		}
		i++;
	}
	if (!path)
		return (ft_strjoin("/", cmd));
	real_path = get_cmds_path(path, cmd);
	free(path);
	if (!real_path)
		return (NULL);
	return (real_path);
}

// Closes pipe file descriptors and waits for both child processes
void	wait_pipes_close(t_cmd *cmd, t_shell *shell, int pid1, int pid2)
{
	close(cmd->pipefd[0]);
	close(cmd->pipefd[1]);
	waitpid(pid1, &shell->status1, 0);
	waitpid(pid2, &shell->status2, 0);
}

// Checks if a given path is a directory
int	is_directory(char *path)
{
	struct stat	path_stat;

	if (access(path, F_OK) == -1)
		return (0);
	if (stat(path, &path_stat) == -1)
	{
		perror("stat error");
		return (0);
	}
	return (S_ISDIR(path_stat.st_mode));
}

// Prints an appropriate error message when command path resolution fails
void	path_errors(t_shell *shell, t_cmd *cmd)
{
	if (ft_strncmp(cmd->argv[0], "sudo", 4) == 0
		&& ft_strlen(cmd->argv[0]) == ft_strlen("sudo"))
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": permission denied\n", 2);
		shell->exit_status = 126;
		return ;
	}
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	shell->exit_status = 127;
	return ;
}
