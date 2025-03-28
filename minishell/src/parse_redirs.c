#include "minishell.h"

// Checks if the current token represents a redirection operator (>, >>, <, <<)
int	special_redirs(char **str)
{
	if (**str == '>')
	{
		(*str)++;
		if (**str == '>')
		{
			return ('+');
		}
		(*str)--;
		return ('>');
	}
	else if (**str == '<')
	{
		(*str)++;
		if (**str == '<')
		{
			return ('-');
		}
		(*str)--;
		return ('<');
	}
	return ('?');
}

// Extracts and allocates the filename string from the token range
char	*aloc_file(char *start_file, t_redir *new_redir,
	char *end_file, t_shell *shell)
{
	int		file_length;
	char	*file;
	char	*temp;

	file_length = end_file - start_file;
	temp = ft_strndup(start_file, file_length);
	file = clean_token(temp, shell, TOKEN);
	if (!file)
	{
		free(new_redir);
		free(temp);
		exit(1);
	}
	free(temp);
	return (file);
}

// Adds a new redirection node to the end of the redirection linked list
t_redir	*add_redir(t_redir *head, int type, t_token *tok, t_shell *shell)
{
	t_redir	*tmp;
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		exit(1);
	memset(new_redir, 0, sizeof(t_redir));
	new_redir->type = type;
	if (type == '-')
		new_redir->file = NULL;
	else
		new_redir->file = aloc_file(tok->start, new_redir, tok->end, shell);
	new_redir->next = NULL;
	if (!head)
		return (new_redir);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
	return (head);
}
