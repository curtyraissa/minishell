/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:45 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:46 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints the value of a variable (after '='), quoting it and escaping '$' if present
static void	print_var_value(char **env_copy, int i, int j)
{
	if (env_copy[i][j] != '\0')
	{
		printf("%c", env_copy[i][j++]);
		printf("\"");
		while (env_copy[i][j])
		{
			if (env_copy[i][j] == '$')
				printf("\\");
			printf("%c", env_copy[i][j++]);
		}
		printf("\"");
	}
}

// Returns the smaller of two given integers
static int	smaller_var(int size1, int size2)
{
	if (size1 < size2)
		return (size1);
	return (size2);
}

// Sorts environment variables alphabetically by name (before '=')
static void	sort_env(char **env)
{
	int		i;
	int		j;
	int		size1;
	int		size2;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[j + 1])
		{
			size1 = var_name_len(env[j]);
			size2 = var_name_len(env[j + 1]);
			if (ft_strncmp(env[j], env[j + 1], smaller_var(size1, size2)) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

// Handles the 'export' builtin when called without arguments,
// printing all environment variables in a sorted, exportable format
int	ft_export_no_args(t_shell *shell)
{
	char	**env_copy;
	int		i;
	int		j;

	env_copy = copy_env(shell->env);
	sort_env(env_copy);
	i = -1;
	while (env_copy[++i])
	{
		j = 0;
		printf("declare -x ");
		while (env_copy[i][j] && env_copy[i][j] != '=')
			printf("%c", env_copy[i][j++]);
		print_var_value(env_copy, i, j);
		printf("\n");
	}
	free_env(env_copy);
	return (0);
}
