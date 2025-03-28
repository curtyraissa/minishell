#include "minishell.h"

// Extracts a string segment without quotes and sets its type ('$' or 'a')
void	chunk_no_quotes(t_chunk **chunk, char **cpy, int size)
{
	(*chunk)->str = ft_strndup(*cpy, size);
	if (**cpy == '$')
		(*chunk)->type = '$';
	else
		(*chunk)->type = 'a';
	if (!(*chunk)->str)
		free(*chunk);
}

// Allocates and initializes a new empty chunk
t_chunk	*init_chunk(void)
{
	t_chunk	*chunk;

	chunk = malloc(sizeof(t_chunk));
	if (!chunk)
		return (NULL);
	chunk->str = NULL;
	chunk->next = NULL;
	return (chunk);
}

// Creates a chunk based on the token input; handles quoted and non-quoted text
t_chunk	*create_chunk(char **tok)
{
	t_chunk	*chunk;
	int		i;
	char	*cpy;

	chunk = init_chunk();
	i = 0;
	cpy = *tok;
	while (cpy[i] && cpy[i] != '"' && cpy[i] != '\'')
	{
		i++;
		(*tok)++;
	}
	if (i > 0)
		chunk_no_quotes(&chunk, &cpy, i);
	else
	{
		chunk->type = **tok;
		while (cpy[i + 1] && cpy[i + 1] != chunk->type)
			i++;
		chunk->str = ft_strndup(cpy + 1, i);
		*tok = cpy + i + 2;
	}
	return (chunk);
}

// Expands a chunk’s content if needed (e.g. variable expansion)
char	*expand_chunk(t_chunk *chunk, t_shell *shell)
{
	char	*tmp;

	tmp = ft_strdup(chunk->str);
	free (chunk->str);
	chunk->str = NULL;
	chunk->str = deal_expansion(tmp, shell);
	free (tmp);
	tmp = NULL;
	return (chunk->str);
}

// Breaks a token into chunks, expands if necessary, and returns final cleaned token
char	*clean_token(char *tok, t_shell *shell, int type)
{
	t_chunk	*chunks;
	t_chunk	*chunk;
	t_chunk	*head;
	char	*tok_cpy;
	char	*final_token;

	tok_cpy = tok;
	chunks = NULL;
	while (*tok_cpy)
	{
		chunk = create_chunk(&tok_cpy);
		if (!chunk)
		{
			free_chunks(head);
			return (NULL);
		}
		if ((chunk->type == '"' || chunk->type == 'a' || chunk->type == '$')
			&& type == TOKEN)
			chunk->str = expand_chunk(chunk, shell);
		chunk_add_back(&chunks, chunk, &head);
	}
	final_token = chunks_join(chunks, shell);
	free_chunks(head);
	return (final_token);
}
