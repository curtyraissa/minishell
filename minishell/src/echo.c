/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:09 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 14:09:48 by rcurty-g         ###   ########.fr       */
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
int	cmd_echo(char **av)
{
	int	i;

	i = 1;
	if (!av[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	while (is_echo_flag(av[i]))
		i++;
	while (av[i] && av[i + 1] != NULL)
	{
		ft_putstr_fd(av[i++], 1);
		ft_putchar_fd(' ', 1);
	}
	if (is_echo_flag(av[1]))
	{
		if (av[i])
			ft_putstr_fd(av[i], 1);
	}
	else
		if (av[i])
			ft_putendl_fd(av[i], 1);
	return (0);
}
