#include "minishell.h"

// Preenche um chunk sem aspas com o texto copiado
void	fill_chunk_no_quotes(t_chunk **chunk, char **cpy, int size)
{
	(*chunk)->str = ft_strndup(*cpy, size);
	if (**cpy == '$')
		(*chunk)->type = '$';
	else
		(*chunk)->type = 'a';
	if (!(*chunk)->str)
		free(*chunk);
}

// Inicializa e retorna um novo chunk
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

// Cria um chunk a partir de um token, considerando aspas
t_chunk	*build_chunk(char **tok)
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
		fill_chunk_no_quotes(&chunk, &cpy, i);
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

// Expande variáveis em um chunk usando o ambiente do shell
char	*expand_chunk_value(t_chunk *chunk, t_shell *shell)
{
	char	*tmp;

	tmp = ft_strdup(chunk->str);
	free (chunk->str);
	chunk->str = deal_expansion(tmp, shell);
	free (tmp);
	return (chunk->str);
}

// Processa e limpa um token, aplicando expansões e juntando os chunks
char	*token_cleanup(char *tok, t_shell *shell, int type)
{
	t_chunk	*chunks = NULL;
	t_chunk	*chunk;
	t_chunk	*head;
	char	*tok_cpy = tok;
	char	*final_token;

	while (*tok_cpy)
	{
		chunk = build_chunk(&tok_cpy);
		if (!chunk)
		{
			clear_chunks(head);
			return (NULL);
		}
		if ((chunk->type == '"' || chunk->type == 'a' || chunk->type == '$')
			&& type == TOKEN)
			chunk->str = expand_chunk_value(chunk, shell);
		append_chunk(&chunks, chunk, &head);
	}
	final_token = join_chunks(chunks, shell);
	clear_chunks(head);
	return (final_token);
}
