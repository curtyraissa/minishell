/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:40 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:04:18 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if any invalid options are passed to a builtin command.
// Returns 1 if an invalid option is found, otherwise returns 0.
int	has_options(char **av, char *command)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if ((ft_strncmp(av[i], "-", 1) == 0 && ft_strlen(av[i]) == 1)
			|| (ft_strncmp(av[i], "--", 2) == 0 && ft_strlen(av[i]) == 2))
			return (0);
		if (av[i][0] == '-')
		{
			printf("minishell: %s: %s: Invalid option\n", command, av[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f')
		return (1);
	else
		return (0);
}

int	mini_error(char *str, int error, t_shell *shell)
{
	shell->exit_status = error;
	if (error == -1)
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	ft_putstr_fd("minishell: syntax error ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

void	on_off_flag(int *flag)
{
	if (*flag == 1)
		*flag = 0;
	else if (*flag == 0)
		*flag = 1;
}
