/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:45:46 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 14:16:32 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the command is a built-in and returns its name if matched
char	*is_builtin(t_cmd *execcmd)
{
	if (ft_strncmp(execcmd->av[0], "echo", 4) == 0
		&& ft_strlen(execcmd->av[0]) == 4)
		return ("echo");
	else if (ft_strncmp(execcmd->av[0], "cd", 2) == 0
		&& ft_strlen(execcmd->av[0]) == 2)
		return ("cd");
	else if (ft_strncmp(execcmd->av[0], "pwd", 3) == 0
		&& ft_strlen(execcmd->av[0]) == 3)
		return ("pwd");
	else if (ft_strncmp(execcmd->av[0], "export", 6) == 0
		&& ft_strlen(execcmd->av[0]) == 6)
		return ("export");
	else if (ft_strncmp(execcmd->av[0], "unset", 5) == 0
		&& ft_strlen(execcmd->av[0]) == 5)
		return ("unset");
	else if (ft_strncmp(execcmd->av[0], "env", 3) == 0
		&& ft_strlen(execcmd->av[0]) == 3)
		return ("env");
	else if (ft_strncmp(execcmd->av[0], "exit", 4) == 0
		&& ft_strlen(execcmd->av[0]) == 4)
		return ("exit");
	return (NULL);
}

// Executes the corresponding built-in function and sets the exit status
void	exec_builtin(char **av, char *builtin, t_shell *shell)
{
	if (ft_strncmp(builtin, "echo", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_echo(av);
	else if (ft_strncmp(builtin, "cd", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_cd(av, shell);
	else if (ft_strncmp(builtin, "pwd", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_pwd(av);
	else if (ft_strncmp(builtin, "export", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_export(av, shell);
	else if (ft_strncmp(builtin, "unset", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_unset(av, shell);
	else if (ft_strncmp(builtin, "env", ft_strlen(builtin)) == 0)
		shell->exit_status = cmd_env(av, shell);
	else if (ft_strncmp(builtin, "exit", ft_strlen(builtin)) == 0)
		cmd_exit(av, shell);
}
