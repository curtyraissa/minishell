/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:37 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 14:11:41 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the given variable exists in the environment
// and does not contain '=' (is a standalone name).
static int	is_var(char **local_env, char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			return (0);
		i++;
	}
	i = 0;
	while (local_env[i])
	{
		if (ft_strncmp(local_env[i], var, ft_strlen(var)) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Removes a variable from the environment array by creating a new array
// without the target variable, then freeing the old one.
static char	**remove_var(char **local_env, char *var)
{
	int		i;
	int		j;
	int		size;
	char	**new_env;

	size = env_size(local_env);
	new_env = malloc(size * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (local_env[i])
	{
		if (ft_strncmp(local_env[i], var, ft_strlen(var)) == 0)
			i++;
		else
			new_env[j++] = ft_strdup(local_env[i++]);
	}
	new_env[j] = NULL;
	free_env(local_env);
	return (new_env);
}

// Implements the 'unset' builtin command.
// Removes variables from the shell environment.
int	cmd_unset(char **av, t_shell *shell)
{
	int	i;

	if (!av[1])
		return (0);
	if (has_options(av, av[0]))
		return (2);
	i = 1;
	while (av[i] != NULL)
	{
		if (is_var(shell->env, av[i]))
			shell->env = remove_var(shell->env, av[i]);
		i++;
	}
	return (0);
}
