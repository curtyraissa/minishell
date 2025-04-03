/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:25 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:04:08 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Redirects STDIN to the input file descriptor while saving the original one
static void	handle_in(t_fds *fds)
{
	if (fds->saved_in == -1)
		fds->saved_in = dup(STDIN_FILENO);
	if (fds->saved_in < 0)
	{
		perror("dup error (saving original stdin)\n");
		exit(1);
	}
	if (dup2(fds->in_fd, STDIN_FILENO) < 0 || fds->saved_in < 0)
	{
		perror("dup2 error (input redirection)\n");
		exit(1);
	}
}

// Redirects STDOUT to the output file descriptor while saving the original one
static void	handle_out(t_fds *fds)
{
	if (fds->saved_out == -1)
		fds->saved_out = dup(STDOUT_FILENO);
	if (fds->saved_out < 0)
	{
		perror("dup error (saving original stdout)\n");
		exit(1);
	}
	if (dup2(fds->out, STDOUT_FILENO) < 0 || fds->saved_out < 0)
	{
		perror("dup2 error (output redirection)\n");
		exit(1);
	}
}

// Handles input redirection for commands, including heredocs and files
int	redirs_in(t_fds *fds, t_redir *redir, t_shell *shell, t_cmd *cmd)
{
	if (redir->type == '-')
	{
		fds->in_fd = cmd->heredoc->pipe_fd[0];
		cmd->heredoc = cmd->heredoc->next;
	}
	else
	{
		if (fds->in_fd != -1)
			close(fds->in_fd);
		fds->in_fd = open(redir->file, O_RDONLY);
		if (fds->in_fd < 0)
			return (mini_error("No such file or directory", -1, shell));
	}
	handle_in(fds);
	return (1);
}

// Handles output redirection for commands, like truncating or appending modes
int	redirs_out(t_fds *fds, t_redir *redir, t_shell *shell)
{
	if (fds->out != -1)
		close(fds->out);
	if (redir->type == '+')
		fds->out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fds->out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->out < 0)
		return (mini_error("No such file or directory", -1, shell));
	handle_out(fds);
	return (1);
}

// Main handler for command redirections:validates and applies before execution
void	handle_redirs(t_cmd *execcmd, t_shell *shell)
{
	t_redir	*redir;

	shell->fds = init_fds();
	redir = execcmd->redir;
	if (ambigous_redir(shell, redir))
		return ;
	while (redir)
	{
		if (file_permissions(shell, redir))
			return ;
		else if (!valid_redir(shell, redir, execcmd))
			return ;
		redir = redir->next;
	}
	if (execcmd->av[0])
		execute_commands(execcmd, shell);
	close_fds(shell->fds);
	free(shell->fds);
	shell->fds = NULL;
}
