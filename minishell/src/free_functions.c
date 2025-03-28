#include "minishell.h"

// Frees the delimiter string of a heredoc and optionally closes its pipe read end.
static void	delete_delimiter(t_heredoc *heredoc, t_shell *shell, int flag)
{
	if (heredoc->delimiter)
	{
		free(heredoc->delimiter);
		heredoc->delimiter = NULL;
	}
	if (shell->heredoc_flag == 1 && flag == 1)
	{
		close(heredoc->pipe_fd[0]);
	}
}

// Recursively frees all heredocs in the command tree.
// Closes file descriptors and deletes each heredoc structure.
void	delete_heredocs(t_shell *shell, int flag, t_cmd *cmd)
{
	t_heredoc	*current;
	t_heredoc	*temp;
	t_cmd		*curr;

	if (!cmd)
		return ;
	curr = cmd;
	if (curr->type == EXEC)
	{
		current = curr->heredoc_head;
		while (current != NULL)
		{
			if (current->fd >= 0)
				close (current->fd);
			delete_delimiter(current, shell, flag);
			temp = current;
			current = current->next;
			free(temp);
		}
	}
	else if (curr->type == PIPE)
	{
		delete_heredocs(shell, flag, curr->left);
		delete_heredocs(shell, flag, curr->right);
	}
}

// Frees a linked list of redirection structures, including file names.
void	free_redirections(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->type != '-')
			free(redir->file);
		free(redir);
		redir = next;
	}
}

// Frees a command structure, its arguments, redirections and subcommands.
void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd == NULL)
		return ;
	i = 0;
	if (cmd->type == EXEC)
	{
		if (cmd->redir)
			free_redirections(cmd->redir);
		while (cmd->argv[i])
			free (cmd->argv[i++]);
		free (cmd->argv);
		free (cmd);
	}
	else if (cmd->type == PIPE)
	{
		free_cmd(cmd->left);
		free_cmd(cmd->right);
		free (cmd);
	}
	cmd = NULL;
}
