/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:29 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 09:59:51 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Normalizes the exit code to a value between 0 and 255
static int	calculate_exit_code(int code)
{
	if (code >= 0)
		return (code % 256);
	else
		return (256 + (code % 256));
}

// Frees the shell and exits with the given code from av[1]
static void	free_and_exit(char **av, t_shell *shell)
{
	int	exit_code;

	exit_code = ft_atoi(*(av + 1));
	free_shell(shell, EXIT_CMD);
	ft_putstr_fd("exit\n", 1);
	exit(calculate_exit_code(exit_code));
}

// Builtin function for 'exit': handles exiting with optional exit status
void	ft_exit(char **av, t_shell *shell)
{
	int	exit_code;

	if (*(av + 1) == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		free_shell(shell, EXIT_CMD);
		exit (0);
	}
	if (ft_strncmp(*(av + 1), "--", 2) == 0
		&& (ft_strlen(*(av + 1)) == ft_strlen("--")))
		return (ft_exit(av + 1, shell));
	if (!valid_code(*(av + 1))
		|| ((*(av + 1))[0] == '-' && !(*(av + 1))[1]))
	{
		exit_code = exit_error(*(av + 1), ERR_NUM);
		free_shell(shell, EXIT_CMD);
		exit(exit_code);
	}
	if (*(av + 2))
	{
		shell->exit_status = exit_error("", ERR_ARG);
		return ;
	}
	free_and_exit(av, shell);
}
