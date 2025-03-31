/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:48 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:49 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new environment variable by stripping '+' from 'VAR+='
// and then updating the shell environment
static void	create_var(t_shell *shell, char *var)
{
	char	*new_var;
	int		i;
	int		j;
	int		flag;

	new_var = malloc(sizeof(char) * ft_strlen(var));
	if (!new_var)
		return ;
	flag = 1;
	i = 0;
	j = 0;
	while (var[i])
	{
		if (var[i] == '+' && flag)
		{
			flag = 0;
			i++;
		}
		new_var[j++] = var[i++];
	}
	new_var[j] = '\0';
	shell->env = update_env(shell->env, new_var);
	free (new_var);
}

// Appends the new value to an existing variable in the environment,
// using '=' if the variable didn't have a value yet
void	append_condition(int *flag, t_shell *shell, char *cpy, int i)
{
	*flag = 0;
	if (ft_strchr(shell->env[i], '='))
		shell->env[i] = ft_strjoin_free(shell->env[i], cpy + 1);
	else
	{
		shell->env[i] = ft_strjoin_free(shell->env[i], "=");
		shell->env[i] = ft_strjoin_free(shell->env[i], cpy + 1);
	}
}

// Handles environment variable appending (VAR+=value).
// If the variable exists, it appends the new value.
// If it doesn't, it creates the variable without '+'.
void	append_var(t_shell *shell, char *var)
{
	int		name_len;
	int		i;
	int		flag;
	char	*cpy;

	cpy = var;
	name_len = 0;
	while (var[name_len] && var[name_len] != '+')
		name_len++;
	while (*cpy && *cpy != '=')
		cpy++;
	flag = 1;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var, name_len) == 0)
		{
			append_condition(&flag, shell, cpy, i);
			return ;
		}
		i++;
	}
	if (flag)
		create_var(shell, var);
}
