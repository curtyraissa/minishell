/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:47:16 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:05:31 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Creates a new av array,skipping empty expansions and duplicating valid strings
char	**copy_av(char **av, int size, int i, int j)
{
	char	**new_av;

	new_av = ft_calloc(sizeof(char *), size + 1);
	if (!new_av)
		return (NULL);
	while (av[i])
	{
		if (av[i] && av[i][0] == '\0')
		{
			new_av[j] = ft_strdup("");
			j ++;
		}
		else if (ft_strncmp(av[i], EXPAND_NULL, ft_strlen(av[i])) != 0)
		{
			new_av[j] = ft_strdup(av[i]);
			if (!new_av[j])
			{
				perror("ft_strdup");
				return (NULL);
			}
			j++;
		}
		i++;
	}
	return (new_av);
}

//Cleans the av array from null expansions and replaces it with a cleaned copy
char	**clean_av(t_cmd *cmd)
{
	int		i;
	int		j;
	int		size;
	char	**new_av;

	i = 0;
	j = 0;
	size = 0;
	while (cmd->av[i])
	{
		if (ft_strncmp(cmd->av[i], EXPAND_NULL, ft_strlen(EXPAND_NULL)) != 0)
			size++;
		i++;
	}
	i = 0;
	new_av = copy_av(cmd->av, size, i, j);
	while (cmd->av[i])
		free (cmd->av[i++]);
	free (cmd->av);
	cmd->ac = size;
	return (new_av);
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
			shell->ac++;
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

//Checks the current character pointed by ptr_str matches any in the given set
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
