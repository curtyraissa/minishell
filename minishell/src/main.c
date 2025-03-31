/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:03 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:47:04 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ctrlc = 0;

// Entry point of the minishell program.
// Initializes shell structure, sets up signals and starts the main loop.
int	main(int argc, char **argv, char *envp[])
{
	t_shell	*shell;

	shell = init_struct(argv, envp);
	if (!shell)
		return (1);
	shell->home_index = var_search(envp, "HOME");
	if (argc != 1 || argv[1])
		return (0);
	signals();
	init_minishell(shell);
	return (0);
}
