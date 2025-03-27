#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>            // printf, perror
# include <unistd.h>           // fork, execve, pipe, dup, dup2, access, isatty, ttyname
# include <stdlib.h>           // malloc, free, exit, getenv
# include <sys/stat.h>         // stat, lstat, fstat
# include <fcntl.h>            // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND
# include <limits.h>           // PATH_MAX
# include <readline/readline.h>  // readline
# include <readline/history.h> // add_history, clear_history
# include <signal.h>           // signal, sigaction, sigemptyset, sigaddset, kill
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

extern int g_signal;  // Global variable for signal control (e.g., Ctrl-C)

/* Redirections */
typedef struct s_redir
{
    int             type;       // Type of redirection (input, output)
    char            *file;      // File path for redirection
    char            *end_file;  // End file in case of range redirection
    struct s_redir  *next;      // Pointer to next redirection
} t_redir;

typedef struct heredoc
{
    int             fd;          // File descriptor for heredoc
    int             index;       // Index for managing multiple heredocs
    int             pipe_fd[2];  // Pipe file descriptors for heredoc input
    char            *delimiter;  // Delimiter string to end heredoc
    struct heredoc  *next;       // Pointer to next heredoc structure
} t_heredoc;

/* command execution */
typedef struct cmd
{
    int             type;       // Type of command (exec, pipe, redir)
    char            **argv;     // Arguments vector
    t_redir         *redir;     // Linked list of redirections
    int             argc;       // Argument count
    int             fd_out;     // File descriptor for output
    int             fd_in;      // File descriptor for input
    t_heredoc       *heredoc;   // Pointer to heredoc structure
    t_heredoc       *heredoc_head; // Pointer to the head of heredoc list
    int             pipefd[2];  // Pipe file descriptors
    int             prev_pipe;  // Indicator of a previous pipe
    struct cmd      *left;      // Pointer to left command in tree (pipes)
    struct cmd      *right;     // Pointer to right command in tree (pipes)
    pid_t           pid1;       // Process ID for the first fork
    pid_t           pid2;       // Process ID for a possible second fork
} t_cmd;

typedef struct s_chunk
{
    char            *str;       // String chunk
    char            type;       // Type of chunk (command, argument)
    struct s_chunk  *next;      // Pointer to next chunk
} t_chunk;

typedef struct token
{
    char    *start;     // Start of the token
    char    *end;       // End of the token
    int     argc;       // Argument count
} t_token;

typedef struct file_descriptors
{
    int     in_fd;      // Input file descriptor
    int     out;        // Output file descriptor
    int     saved_in;   // Saved standard input
    int     saved_out;  // Saved standard output
} t_fds;

typedef struct shell
{
    t_cmd   *head;          // Head of command list
    char    *name;          // Name of the shell
    char    **env;          // Environment variables
    char    *input;         // User input
    char    *prompt;        // Prompt string
    int     ambiguous;      // Flag for ambiguous redirections
    int     argc;           // Argument count
    int     heredoc_name;   // Index for naming heredocs
    int     status1;        // Status of first process
    int     status2;        // Status of second process
    int     home_index;     // Index of HOME in environment
    int     exit_heredoc;   // Flag for exiting heredoc
    int     exit_status;    // Exit status of the shell
    int     heredoc_flag;   // Flag for heredoc processing
    t_fds   *fds;           // File descriptors
} t_shell;

/* Function prototypes  */
void		fill_chunk_no_quotes(t_chunk **chunk, char **cpy, int size);
t_chunk		*init_chunk(void);
t_chunk		*build_chunk(char **tok);
char		*expand_chunk_value(t_chunk *chunk, t_shell *shell);
char		*token_cleanup(char *tok, t_shell *shell, int type);
int			final_token_size(t_chunk *chunks);
t_chunk		*chunk_last(t_chunk *chunk);
void		append_chunk(t_chunk **chunks, t_chunk *chunk, t_chunk **head);
void		clear_chunks(t_chunk *chunks);
char		*join_chunks(t_chunk *chunks, t_shell *shell);


#endif
