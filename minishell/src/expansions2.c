/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:39 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 14:45:31 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Extracts the name of the variable to be expanded (e.g. HOME, USER, ?)
char	*create_expand(char *expand)
{
	int		i;
	char	*str;

	str = NULL;
	i = 0;
	while (expand[i] && (ft_isalnum(expand[i])
			|| expand[i] == '_' || expand[i] == '?'))
	{
		if (expand[i] == '?')
		{
			i++;
			break ;
		}
		i++;
	}
	str = ft_strndup(expand, i);
	return (str);
}

// Updates shell exit status if $? is used and last signal was SIGINT
void	return_exit_code(char **token, t_shell *shell)
{
	char	*final_token;

	final_token = create_expand(*token);
	if (ft_strncmp(final_token, "?", 1) == 0)
	{
		if (g_signal == SIGINT)
			shell->exit_status = 130;
	}
	return (free(final_token));
}

// Handles special expansion cases: $?, $$, $0, and numeric parameters
char	*expand_cases(char **token, t_shell *shell)
{
	if (**token == '?')
	{
		return_exit_code(token, shell);
		(*token)++;
		return (ft_itoa(shell->exit_status));
	}
	else if (**token == '0')
		return ((*token)++, ft_strdup(shell->name));
	else if (**token == '$')
		return ((*token)++, ft_itoa(getpid()));
	else if (ft_isdigit(**token))
		return ((*token)++, ft_strdup(""));
	return (ft_strdup(""));
}

// Joins a portion of the token (until next '$') to the current final result
char	*expansion_join(char *token, char *old_final, int *i)
{
	char	*tmp;
	char	*new_final;

	new_final = NULL;
	while (token[*i] && token[*i] != '$')
		(*i)++;
	tmp = ft_strndup(token, *i);
	new_final = ft_strjoin_free(old_final, tmp);
	free(tmp);
	tmp = NULL;
	return (new_final);
}
