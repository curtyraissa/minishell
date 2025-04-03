/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:45:53 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:16:14 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles path substitution when "cd ~" is used
static	int	cd_home_path(char **av, t_shell *shell)
{
	char	**env;
	char	*path;
	int		home_index;

	env = shell->env;
	home_index = var_search(shell->env, "HOME");
	if (home_index >= 0)
		path = ft_strjoin(env[home_index] + 5, av[1] + 1);
	else
	{
		if (shell->home_index != -1)
			path = ft_strjoin(getenv("HOME"), av[1] + 1);
		else
			return (1);
	}
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (free (path), 1);
	}
	free (path);
	return (0);
}

// Handles behavior for "cd -" (return to previous directory)
static	int	cd_minus(t_shell *shell)
{
	char	**env;
	char	path[4096];

	env = shell->env;
	if (var_is_set(shell->env, "OLDPWD"))
	{
		if (chdir(env[var_search(shell->env, "OLDPWD")] + 7) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
		getcwd(path, sizeof(path));
		printf("%s\n", path);
	}
	else
		return (1);
	return (0);
}

// Decides which cd behavior to apply based on the argument
static int	change_dir(char **av, t_shell *shell)
{
	if (av[1][0] == '~')
		return (cd_home_path(av, shell));
	else if (ft_strncmp(av[1], "-", 1) == 0 && ft_strlen(av[1]) == 1)
		return (cd_minus(shell));
	else if (av[1][0] && chdir(av[1]) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

// Builtin function for 'cd': changes the working directory
int	cmd_cd(char **av, t_shell *shell)
{
	char	**env;
	char	path[4096];
	char	*tmp;

	env = shell->env;
	if (has_options(av, "cd"))
		return (2);
	if (shell->ac > 1 && av[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	tmp = ft_getcwd(shell);
	if (!av[1] || (ft_strncmp(av[1], "--", 2) == 0
			&& ft_strlen(av[1]) == 2))
	{
		if (var_is_set(shell->env, "HOME"))
			chdir(env[var_search(shell->env, "HOME")] + 5);
		else
			return (1);
	}
	else
		if (change_dir(av, shell) == 1)
			return (1);
	update_var(shell, "OLDPWD", tmp);
	update_var(shell, "PWD", getcwd(path, sizeof(path)));
	return (0);
}
