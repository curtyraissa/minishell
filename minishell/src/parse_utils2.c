/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:16 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:47:17 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new argv array, skipping empty expansions and duplicating valid strings
char	**copy_argv(char **argv, int size, int i, int j)
{
	char	**new_argv;

	new_argv = ft_calloc(sizeof(char *), size + 1);
	if (!new_argv)
		return (NULL);
	while (argv[i])
	{
		if (argv[i] && argv[i][0] == '\0')
		{
			new_argv[j] = ft_strdup("");
			j ++;
		}
		else if (ft_strncmp(argv[i], EXPAND_NULL, ft_strlen(argv[i])) != 0)
		{
			new_argv[j] = ft_strdup(argv[i]);
			if (!new_argv[j])
			{
				perror("ft_strdup");
				return (NULL);
			}
			j++;
		}
		i++;
	}
	return (new_argv);
}

// Cleans the argv array from null expansions and replaces it with a cleaned copy
char	**clean_argv(t_cmd *cmd)
{
	int		i;
	int		j;
	int		size;
	char	**new_argv;

	i = 0;
	j = 0;
	size = 0;
	while (cmd->argv[i])
	{
		if (ft_strncmp(cmd->argv[i], EXPAND_NULL, ft_strlen(EXPAND_NULL)) != 0)
			size++;
		i++;
	}
	i = 0;
	new_argv = copy_argv(cmd->argv, size, i, j);
	while (cmd->argv[i])
		free (cmd->argv[i++]);
	free (cmd->argv);
	cmd->argc = size;
	return (new_argv);
}

// Updates quote tracking state when entering or exiting quoted text
static void	quote_check(char *str, int *in_quotes, char *quote_type)
{
	if (!(*in_quotes))
	{
		*in_quotes = 1;
		*quote_type = *str;
	}
	else
		*in_quotes = 0;
}

// Counts how many tokens are in the given string (excluding pipes)
void	token_count(char *str, t_shell *shell)
{
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	while (*str)
	{
		while (is_whitespace(*str) && !in_quotes)
			str++;
		if (*str && !is_whitespace(*str) && *str != '|')
			shell->argc++;
		while (*str && (in_quotes || (!is_whitespace(*str) && *str != '|')))
		{
			if ((*str == '\'' || *str == '"')
				&& (!in_quotes || *str == quote_type))
				quote_check(str, &in_quotes, &quote_type);
			str++;
		}
		if (*str == '|' && !in_quotes)
			return ;
	}
}

// Checks if the current character pointed by ptr_str matches any in the given set
int	find_char(char **ptr_str, char *set)
{
	char	*s;

	if (!ptr_str || !*ptr_str || !set)
		return (0);
	s = *ptr_str;
	if (*s)
	{
		while (*s && is_whitespace(*s))
			s++;
	}
	*ptr_str = s;
	if (*s && ft_strchr(set, *s))
		return (1);
	else
		return (0);
}
