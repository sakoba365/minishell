#include "libft/libft.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>
#if !defined (PARAMS)
#    define PARAMS(protos) ()
#endif

typedef int shell_func_t PARAMS((char *));
extern char **environ;


/* The names of functions that execute the shell */
int shell_echo PARAMS((char *));
int shell_exit PARAMS((char *));
int shell_pwd PARAMS((char *));
int shell_env PARAMS((char *));
int shell_export PARAMS((char *));
int shell_unset PARAMS((char *));
int shell_cd PARAMS((char *));

/* A structure which contains information on the build-in commands. */

typedef struct {
  char *name;     /* User printable name of the function. */
  shell_func_t *func;   /* Function to call to do the job. */
  char *doc;      /* Documentation for this function.  */
} COMMAND;

static COMMAND commands[] = {
  { "echo", shell_echo, "Print user input" },
  { "exit", shell_exit, "Exit Minishell" },
  { "pwd", shell_pwd, "Show current directory" },
  { "env", shell_env, "Print environment variables" },
  { "export", shell_export, "Print all exported variables." },
  { "unset", shell_unset, "delete the variables during program execution" },
  { "cd", shell_cd, "change the current working directory" },
  { (char *)NULL, (shell_func_t *)NULL, (char *)NULL }
};



char **parseArgs(char *args);

/* Argument checker */
int valid_argument (char *caller, char *arg);

/* The name of this program, as taken from argv[0]. */
char *progname;

int done;

# define WHITE_SPACE " \t\n\v\f\r"
#ifndef is_whitespace
#define is_whitespace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n') || ((c) == '\v') || ((c) == '\f') || ((c) == '\r'))
#endif
#ifndef is_quote
#define is_quote(c) (((c) == '"') || ((c) == '\''))
#endif
#ifndef is_single_quote
#define is_single_quote(c) ((c) == '\'')
#endif
#ifndef is_delim_token
#define is_delim_token(c) (((c) == ';') || ((c) == '|') || ((c) == '&') || ((c) == '>') || ((c) == '<'))
#endif
#ifndef is_delim_cmd
#define is_delim_cmd(c) (((c) == ';') || ((c) == '|') || ((c) == '&'))
#endif
#ifndef is_redirect
#define is_redirect(c) (((c) == '>') || ((c) == '<'))
#endif
#ifndef is_absolute
#define is_absolute(c) (((c) == '/'))
#endif
#ifndef is_relative
#define is_relative(c) (((c) == '.'))
#endif
#ifndef is_blank
#define is_blank(c) (((c) == '\0'))
#endif


typedef struct parser_token
{
  char *data;
  int  *position;
} t_token;
typedef struct s_cmd
{
  t_list    *tokens;
  t_list    *redirs;
}t_cmd;
typedef struct s_redir
{
  char      *direction;
  char      type[2];
}       t_redir;
typedef struct s_cmd_table
{
  t_list      *cmds;
  char      *delimiter;
  int       nb_cmds;
  int       **pipes;
  t_list      *pids;
  int       return_value;
} t_cmd_table;
typedef struct s_ast
{
  t_list      *cmd_tables;
}       t_ast;

typedef struct s_lexer{
  t_list      *cmd_tables;
} t_lexer;

COMMAND *find_command (char *name);
void release_array(void **ptr);
void init_minishell();
void  env_to_list(t_list **dup_envp);
t_list *get_lexer(const char *input);
void run_lexer(t_list *lexer);
void  set_redirs_pipes(t_list *redirs,
            t_cmd_table *cmd_table,
            int process_index);
void  filt_envs(t_list **tokens);

t_list *dup_envp;
int exit_status;
extern char **environ;
void  sa_del(void *data);