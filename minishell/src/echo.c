/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:09 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:10 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks whether the given argument is a valid '-n' flag for echo
static int	is_echo_flag(char *arg)
{
	int	i;

	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	if (!arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// Builtin function for 'echo': prints arguments to stdout
int	ft_echo(char **argv)
{
	int	i;

	i = 1;
	if (!argv[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	while (is_echo_flag(argv[i]))
		i++;
	while (argv[i] && argv[i + 1] != NULL)
	{
		ft_putstr_fd(argv[i++], 1);
		ft_putchar_fd(' ', 1);
	}
	if (is_echo_flag(argv[1]))
	{
		if (argv[i])
			ft_putstr_fd(argv[i], 1);
	}
	else
		if (argv[i])
			ft_putendl_fd(argv[i], 1);
	return (0);
}
