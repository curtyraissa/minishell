/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:00 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 14:45:44 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees memory and resets values from the shell structure, optionally fully cleaning
void	free_shell(t_shell *shell, int i)
{
	free (shell->prompt);
	free (shell->input);
	g_signal = 0;
	shell->ambiguous = 0;
	if (i)
	{
		free_env(shell->env);
		if (i == EXIT_EOF)
			printf("exit\n");
		else if (i == EXIT_CMD)
		{
			if (shell->head)
				free_cmd(shell->head);
			if (shell->fds)
			{
				close_fds(shell->fds);
				free (shell->fds);
			}
		}
		free (shell);
		return ;
	}
	shell->prompt = NULL;
	shell->input = NULL;
}

// Builds and returns the custom shell prompt string with color formatting
static char	*get_prompt(void)
{
	char	*prompt;

	prompt = ft_strjoin_free(ft_strdup("\033[1;34m"), "");
	prompt = ft_strjoin_free(prompt, "minishell");
	prompt = ft_strjoin_free(prompt, "$\033[0m ");
	return (prompt);
}

// Entry loop for minishell: reads input, checks syntax, runs commands
void	init_minishell(t_shell *shell)
{
	t_cmd	*cmd;

	while (1)
	{
		shell->prompt = get_prompt();
		shell->input = readline(shell->prompt);
		if (!shell->input)
		{
			free_shell(shell, EXIT_EOF);
			break ;
		}
		add_history(shell->input);
		if (syntax_check(shell) == 0)
		{
			cmd = parse_cmd(shell->input, shell);
			shell->head = cmd;
			if (handle_heredoc(cmd, shell) == 0)
				run_cmd(cmd, shell);
			delete_heredocs(shell, 1, shell->head);
			free_cmd(shell->head);
		}
		free_shell(shell, 0);
	}
	rl_clear_history();
	exit (0);
}
