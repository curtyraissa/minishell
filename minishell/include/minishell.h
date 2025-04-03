/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:44:42 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/04/03 10:16:19 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>             // printf, perror
# include <unistd.h> // fork, execve, pipe, dup, dup2, access, isatty, ttyname
# include <stdlib.h>           // malloc, free, exit, getenv
# include <sys/stat.h>         // stat, lstat, fstat
# include <fcntl.h>    // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND
# include <limits.h>           // PATH_MAX
# include <readline/readline.h>  // readline
# include <readline/history.h> // add_history, clear_history
# include <signal.h>        // signal, sigaction, sigemptyset, sigaddset, kill
# include "../libft/libft.h"
# include <sys/types.h>        // pid_t, size_t
# include <sys/wait.h>         // wait, waitpid

# define EXEC  1   // Represents an executable command
# define REDIR 2   // Represents a redirection command
# define PIPE  3   // Represents a pipe command
# define EXPORT_NEW  1        // New export without appending
# define EXPORT_APPEND 2      // Append to existing export
# define EXPAND_NULL "\x02"   // Non-printable character used as a placeholder
# define ERR_NUM  1  // Error number for generic issues
# define ERR_ARG 2   // Error number for argument related issues
# define EXIT_EOF 10  // Exit due to an EOF (End of File)
# define EXIT_CMD 11  // Exit due to a command
# define HEREDOC 1   // Flag for heredoc processing
# define TOKEN 0     // Flag for token processing

# define RESET  "\033[0m"
# define RED    "\033[1;31m"
# define GREEN  "\033[1;32m"

extern int	g_signal;// Global variable for signal control (e.g., Ctrl-C)

/* Redirections */
typedef struct s_redir
{
	char				*file; // File path for redirection
	char				*end_file; // End file in case of range redirection
	struct s_redir		*next; // Pointer to next redirection
	int					type; // Type of redirection (input, output)
}	t_redir;

typedef struct heredoc
{
	int					fd; // File descriptor for heredoc
	int					index; // Index for managing multiple heredocs
	int					pipe_fd[2]; // Pipe file descriptors for heredoc input
	char				*delimiter; // Delimiter string to end heredoc
	struct heredoc		*next; // Pointer to next heredoc structure
}	t_heredoc;

/* command execution */
typedef struct cmd
{
	int					type; // Type of command (exec, pipe, redir)
	char				**av; // Arguments vector
	t_redir				*redir; // Linked list of redirections
	int					ac; // Argument count
	int					fd_out; // File descriptor for output
	int					fd_in; // File descriptor for input
	t_heredoc			*heredoc; // Pointer to heredoc structure
	t_heredoc			*heredoc_head; // Pointer to the head of heredoc list
	int					pipefd[2]; // Pipe file descriptors
	int					prev_pipe; // Indicator of a previous pipe
	struct cmd			*left; // Pointer to left command in tree (pipes)
	struct cmd			*right; // Pointer to right command in tree (pipes)
	pid_t				pid1; // Process ID for the first fork
	pid_t				pid2; // Process ID for a possible second fork
}	t_cmd;

typedef struct s_chunk
{
	char				*str; // String chunk
	char				type; // Type of chunk (command, argument)
	struct s_chunk		*next; // Pointer to next chunk
}	t_chunk;

typedef struct token
{
	char				*start; // Start of the token
	char				*end; // End of the token
	int					ac; // Argument count
}	t_token;

typedef struct file_descriptors
{
	int					in_fd; // Input file descriptor
	int					out; // Output file descriptor
	int					saved_in; // Saved standard input
	int					saved_out; // Saved standard output
}	t_fds;

typedef struct shell
{
	t_cmd				*head; // Head of command list
	char				*name; // Name of the shell
	char				**env; // Environment variables
	char				*input; // User input
	char				*prompt; // Prompt string
	int					ambiguous; // Flag for ambiguous redirections
	int					ac; // Argument count
	int					heredoc_name; // Index for naming heredocs
	int					status1; // Status of first process
	int					status2; // Status of second process
	int					home_index; // Index of HOME in environment
	int					exit_heredoc; // Flag for exiting heredoc
	int					exit_status; // Exit status of the shell
	int					heredoc_flag; // Flag for heredoc processing
	t_fds				*fds; // File descriptors
}	t_shell;

/* Function prototypes  */
void		init_minishell(t_shell *shell);
t_token		*create_token(void);
t_shell		*init_struct(char **av, char **envp);
t_fds		*init_fds(void);
t_cmd		*create_cmd(t_shell *shell, int type, t_cmd *left, t_cmd *right);
void		signals(void);
void		signal_handler(int sig);
void		heredoc_sig_handler(int sig);
void		child_signal_handler(int sig);
char		**copy_env(char **env);
int			env_size(char **env);
void		free_env(char **env);
int			var_name_len(char *var);
int			var_search(char **env, char *var);
t_cmd		*parse_cmd(char *str, t_shell *shell);
int			get_token(char **ptr_str, char **start_token, char **end_token);
int			special_chars(char **str);
int			find_char(char **ptr_str, char *set);
int			syntax_check(t_shell *shell);
void		token_count(char *str, t_shell *ac);
t_redir		*add_redir(t_redir *head, int type, t_token *tok, t_shell *shell);
int			deal_token(t_cmd *cmd, char **str,
				t_token *token, t_shell *shell);
char		*clean_token(char *tok, t_shell *shell, int type);
int			final_token_size(t_chunk *chunks);
t_chunk		*chunk_last(t_chunk *chunk);
void		chunk_add_back(t_chunk **chunks, t_chunk *chunk, t_chunk **head);
char		*chunks_join(t_chunk *chunks, t_shell *shell);
void		free_chunks(t_chunk *chunks);
char		**clean_av(t_cmd *cmd);
char		*deal_expansion(char *token, t_shell *shell);
char		*expansion_join(char *token, char *old_final, int *i);
char		*expand_cases(char **token, t_shell *shell);
char		*create_expand(char *expand);
void		run_cmd(t_cmd *cmd, t_shell *shell);
void		execute_commands(t_cmd *execcmd, t_shell *shell);
void		handle_child_process(char *path, t_cmd *execcmd,
				t_shell *shell);
void		handle_parent_process(int pid, char *path,
				t_cmd *execcmd, t_shell *shell);
int			fork_function1(t_cmd *pipecmd, t_shell *shell);
int			fork_function2(t_cmd *pipecmd, t_shell *shell);
void		wait_pipes_close(t_cmd *cmd, t_shell *shell, int pid1, int pid2);
char		*get_cmd_path(char **env, char *cmd);
char		*get_cmds_path(char *path, char *cmd);
int			is_directory(char *path);
void		path_errors(t_shell *shell, t_cmd *cmd);
int			has_slash(char *arg);
void		handle_redirs(t_cmd *execcmd, t_shell *shell);
int			handle_heredoc(t_cmd *cmd, t_shell *shell);
int			heredoc_loop(t_heredoc *curr, t_shell *shell);
int			process_heredoc(t_heredoc *curr, t_shell *shell);
t_heredoc	*get_delimiter(char *start_tok, char *end_tok,
				t_shell *shell, t_cmd *cmd);
int			special_redirs(char **str);
int			ambigous_redir(t_shell *shell, t_redir *redir);
int			file_permissions(t_shell *shell, t_redir *redir);
int			valid_redir(t_shell *shell, t_redir *redir, t_cmd *cmd);
int			redirs_in(t_fds *fds, t_redir *redir, t_shell *shell, t_cmd *cmd);
int			redirs_out(t_fds *fds, t_redir *redir, t_shell *shell);
void		close_fds(t_fds *fds);
void		free_cmd(t_cmd *cmd);
void		delete_heredocs(t_shell *shell, int flag, t_cmd *cmd);
void		free_split(char **split);
void		free_shell(t_shell *shell, int i);
char		*is_builtin(t_cmd *execcmd);
void		exec_builtin(char **av, char *builtin, t_shell *shell);
int			ft_echo(char **av);
int			ft_pwd(char **av);
int			cmd_cd(char **av, t_shell *shell);
int			var_is_set(char **local_env, char *var);
char		*ft_getcwd(t_shell *shell);
int			update_var(t_shell *shell, char *var_name, char *var_value);
int			ft_export(char **av, t_shell *shell);
char		**update_env(char **local_env, char *var);
int			ft_export_no_args(t_shell *shell);
void		append_var(t_shell *shell, char *var);
int			ft_unset(char **av, t_shell *shell);
int			ft_env(char **av, t_shell *shell);
void		ft_exit(char **av, t_shell *shell);
int			exit_error(char *arg, int error);
int			valid_code(char *arg);
int			has_options(char **av, char *command);
int			mini_error(char *str, int error, t_shell *shell);
int			is_whitespace(char c);
void		on_off_flag(int *flag);

#endif
