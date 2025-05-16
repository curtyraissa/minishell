/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:32 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:33 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles exit-related error messages and returns appropriate status code
int	exit_error(char *arg, int error)
{
	if (error == ERR_ARG)
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (error == ERR_NUM)
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (255);
	}
	return (0);
}

// Checks if the character 'c' would cause overflow when added to 'num'
static int	llong_range(int sign, long long num, char c)
{
	if (num * sign > LLONG_MAX / 10 || num * sign < LLONG_MIN / 10)
		return (0);
	if ((num * sign == LLONG_MAX / 10 && c - '0' > LLONG_MAX % 10)
		|| (num * sign == LLONG_MIN / 10 && c - '0' > -(LLONG_MIN % 10)))
		return (0);
	return (1);
}

// Validates whether the given string is a valid numeric argument for exit
int	valid_code(char *arg)
{
	int			sign;
	int			i;
	long long	num;

	num = 0;
	i = 0;
	sign = 1;
	while (arg[i] && is_whitespace(arg[i]))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		if (arg[i++] == '-')
			sign *= -1;
	while (arg[i] && ft_isdigit(arg[i]))
	{
		if (llong_range(sign, num, arg[i]) == 0)
			return (0);
		num = num * 10 + arg[i++] - '0';
	}
	while (arg[i] && is_whitespace(arg[i]))
		i++;
	if (arg[i])
		return (0);
	return (1);
}
