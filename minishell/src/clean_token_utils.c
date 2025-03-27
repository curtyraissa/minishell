#include "minishell.h"

// Calcula o tamanho total necessário para juntar todos os chunks
int	final_token_size(t_chunk *chunks)
{
	int	i = 0;

	while (chunks && chunks->str)
	{
		i += ft_strlen(chunks->str);
		chunks = chunks->next;
	}
	return (i);
}

// Retorna o último chunk da lista encadeada
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

// Adiciona um chunk no final da lista de chunks
void	append_chunk(t_chunk **chunks, t_chunk *chunk, t_chunk **head)
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

// Libera toda a memória alocada para a lista de chunks
void	clear_chunks(t_chunk *chunks)
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

// Junta todos os chunks em uma única string final
char	*join_chunks(t_chunk *chunks, t_shell *shell)
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
