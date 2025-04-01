/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:42 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/01 09:53:40 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Validates the name of the environment variable and 
determines if it's a new or append operation*/
static int	valid_name_len(t_shell *shell, char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
	{
		shell->exit_status = 1;
		return (0);
	}
	i++;
	while (arg[i])
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
			return (EXPORT_APPEND);
		if (arg[i] == '=')
			break ;
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			shell->exit_status = 1;
			return (0);
		}
		i++;
	}
	return (EXPORT_NEW);
}

// Updates an existing variable in the local environment if it exists
static int	var_update(char **local_env, char *var)
{
	int	i;
	int	var_size;

	var_size = 0;
	while (var[var_size] && var[var_size] != '=')
		var_size++;
	i = 0;
	while (local_env[i])
	{
		if (!ft_strchr(var, '=')
			&& ft_strncmp(local_env[i], var, ft_strlen(var)) == 0)
			return (1);
		if (ft_strncmp(local_env[i], var, var_size) == 0)
		{
			free (local_env[i]);
			local_env[i] = NULL;
			local_env[i] = ft_strdup(var);
			return (1);
		}
		i++;
	}
	return (0);
}

// Appends a new variable to the local environment array
char	**update_env(char **local_env, char *var)
{
	int		i;
	int		local_env_size;
	char	**new_env;

	if (var_update(local_env, var))
		return (local_env);
	local_env_size = env_size(local_env);
	new_env = malloc((local_env_size + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (local_env[i])
	{
		new_env[i] = ft_strdup(local_env[i]);
		i++;
	}
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	free_env(local_env);
	return (new_env);
}

// Prints an error message for invalid identifiers in export
static void	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

/*Builtin function for 'export': handles variable exportation 
with validation and appending*/
int	ft_export(char **argv, t_shell *shell)
{
	int	i;
	int	flag;

	if (argv[1] == NULL)
		return (ft_export_no_args(shell));
	if (has_options(argv, argv[0]))
		return (2);
	i = 0;
	flag = 0;
	while (argv[++i] != NULL)
	{
		if (valid_name_len(shell, argv[i]) == EXPORT_NEW)
			shell->env = update_env(shell->env, argv[i]);
		else if (valid_name_len(shell, argv[i]) == EXPORT_APPEND)
			append_var(shell, argv[i]);
		else if (!valid_name_len(shell, argv[i]))
		{
			export_error(argv[i]);
			flag = 1;
		}
	}
	if (flag)
		return (1);
	return (0);
}
