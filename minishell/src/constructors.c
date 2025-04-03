/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:06 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:05:00 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Creates and init. a new command structure with optional left/right children
t_cmd	*create_cmd(t_shell *shell, int type, t_cmd *left, t_cmd *right)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit(0);
	ft_memset(cmd, 0, sizeof(t_cmd));
	if (type == EXEC)
	{
		cmd->av = ft_calloc(sizeof(char *), (shell->ac + 1));
		if (!cmd->av)
			return (NULL);
	}
	cmd->ac = 0;
	cmd->heredoc = NULL;
	cmd->heredoc_head = NULL;
	cmd->left = left;
	cmd->right = right;
	cmd->type = type;
	return (cmd);
}

// Allocates and initializes a new token structure
t_token	*create_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		exit(0);
	ft_memset(token, 0, sizeof(t_token));
	token->ac = 0;
	return (token);
}

// Initializes the shell structure with default values and environment copy
t_shell	*init_struct(char **av, char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->head = NULL;
	shell->ambiguous = 0;
	shell->heredoc_flag = 0;
	shell->heredoc_name = 0;
	shell->exit_heredoc = 0;
	shell->name = av[0] + 2;
	shell->exit_status = 0;
	shell->ac = 0;
	shell->status1 = 0;
	shell->status2 = 0;
	shell->prompt = NULL;
	shell->input = NULL;
	shell->fds = NULL;
	shell->env = copy_env(envp);
	if (!shell->env)
		return (NULL);
	return (shell);
}

// Allocates and initializes file descriptor tracking structure
t_fds	*init_fds(void)
{
	t_fds	*fds;

	fds = malloc(sizeof(t_fds));
	fds->in_fd = -1;
	fds->out = -1;
	fds->saved_in = -1;
	fds->saved_out = -1;
	return (fds);
}
