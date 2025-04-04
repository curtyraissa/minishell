/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:46:00 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/01 10:01:20 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Calculates the total length of all chunk strings combined
int	final_token_size(t_chunk *chunks)
{
	int	i;

	i = 0;
	while (chunks && chunks->str)
	{
		i += ft_strlen(chunks->str);
		chunks = chunks->next;
	}
	return (i);
}

// Returns the last chunk in a linked list of chunks
t_chunk	*chunk_last(t_chunk *chunk)
{
	if (!chunk || !chunk->str)
		return (NULL);
	while (chunk && chunk->str)
	{
		if (chunk->next)
			chunk = chunk->next;
		else
			break ;
	}
	return (chunk);
}

/*Appends a chunk to the end of the chunk list; 
updates the head if list was empty*/
void	chunk_add_back(t_chunk **chunks, t_chunk *chunk, t_chunk **head)
{
	t_chunk	*last;

	last = chunk_last(*chunks);
	if (!last)
	{
		*chunks = chunk;
		*head = chunk;
	}
	else
		last->next = chunk;
}

// Frees all memory allocated for a linked list of chunks
void	free_chunks(t_chunk *chunks)
{
	t_chunk	*tmp;

	while (chunks)
	{
		tmp = chunks->next;
		free (chunks->str);
		free (chunks);
		chunks = tmp;
	}
}

/*Joins all chunk strings into one final string, 
handling edge cases (e.g., expansion)*/
char	*chunks_join(t_chunk *chunks, t_shell *shell)
{
	char	*str;

	str = ft_calloc(sizeof(char), final_token_size(chunks) + 1);
	if (!str)
		return (NULL);
	while (chunks && chunks->str)
	{
		if (ft_strncmp(chunks->str, EXPAND_NULL, 1) == 0
			&& ft_strlen(chunks->str) == ft_strlen(EXPAND_NULL))
		{
			shell->ambiguous = 1;
			str = ft_strjoin_free(str, "");
		}
		else if (chunks->type == '$' && chunks->str[0] == '$'
			&& ft_strlen(chunks->str) == 1 && chunks->next)
			str = ft_strjoin_free(str, "");
		else
			str = ft_strjoin_free(str, chunks->str);
		chunks = chunks->next;
	}
	return (str);
}
