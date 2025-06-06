/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:35 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:46:36 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles shell variable expansion logic (e.g., $HOME, $?, $$, etc.)
char	*is_expansion(char **token, t_shell *shell)
{
	int		i;
	char	*tmp;
	char	*expand;

	tmp = *token;
	expand = NULL;
	if (**token == '$')
	{
		(*token)++;
		if (is_whitespace(**token) || **token == '\0'
			|| **token == '\'' || **token == '"')
			return (ft_strdup("$"));
		if (**token == '0' || **token == '$'
			|| ft_isdigit(**token) || **token == '?')
			return (expand_cases(token, shell));
		expand = create_expand(*token);
		*token = *token + ft_strlen(expand);
		i = var_search(shell->env, expand);
		if (i >= 0)
			return (free(expand), ft_strdup(shell->env[i]
					+ (var_name_len(shell->env[i]) + 1)));
		else
			return (free(expand), ft_strdup(EXPAND_NULL));
	}
	return (ft_strdup(tmp));
}

// Joins the expansion result into final string, handling null cases specially
void	expand_null_case(char **final, char *tmp)
{
	if (ft_strncmp(*final, EXPAND_NULL, 1) == 0
		&& (ft_strlen(*final) == ft_strlen(EXPAND_NULL))
		&& ft_strncmp(tmp, EXPAND_NULL, 1) == 0
		&& (ft_strlen(tmp) == ft_strlen(EXPAND_NULL)))
		*final = ft_strjoin_free(*final, "");
	else
		*final = ft_strjoin_free(*final, tmp);
}

// Removes all expansion-null placeholders from the final expanded string
char	*clean_expansion(char *str)
{
	int		i;
	int		j;
	int		size;
	char	*final;

	i = -1;
	size = 0;
	while (str[++i])
	{
		if (str[i] != EXPAND_NULL[0])
			size++;
	}
	final = ft_calloc(sizeof(char), size + 1);
	if (!final)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != EXPAND_NULL[0])
			final[j++] = str[i];
		i++;
	}
	free (str);
	return (final);
}

// Main function for processing and applying all expansions within a token
char	*deal_expansion(char *token, t_shell *shell)
{
	int		i;
	char	*final;
	char	*tmp;

	tmp = NULL;
	final = ft_calloc(sizeof(char), 1);
	while (*token)
	{
		if (*token && *token == '$')
		{
			tmp = is_expansion(&token, shell);
			expand_null_case(&final, tmp);
			free(tmp);
			tmp = NULL;
		}
		else
		{
			i = 0;
			final = expansion_join(token, final, &i);
			token = token + i;
		}
	}
	if (ft_strlen(final) > 1)
		final = clean_expansion(final);
	return (final);
}
