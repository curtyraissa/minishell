#include "minishell.h"

// Builtin function for 'env': prints all environment variables that contain '='
// If any arguments are passed, it shows an error and returns 1
int	ft_env(char **argv, t_shell *shell)
{
	int		i;
	char	*equal;
	char	*arg;

	if (argv[1])
	{
		printf("minishell: env: No options or arguments are allowed\n");
		return (1);
	}
	i = 0;
	while (shell->env[i])
	{
		arg = shell->env[i];
		equal = ft_strchr(arg, '=');
		if (equal)
			printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}
