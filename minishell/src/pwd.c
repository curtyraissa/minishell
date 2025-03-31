/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:21 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:47:22 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Implements the 'pwd' builtin command.
// Prints the current working directory to stdout.
// Returns 2 if any options are passed, otherwise 0.
int	ft_pwd(char **argv)
{
	char	path[4096];

	if (has_options(argv, argv[0]))
		return (2);
	if (getcwd(path, sizeof(path)))
		ft_putendl_fd(path, 1);
	return (0);
}
