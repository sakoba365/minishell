#include "minishell.h"


void  env_to_list(t_list **dup_envp)
{
  t_list  *list_env;
  int   i;
  char  *tmp;

  if (!environ || !environ[0])
    exit(EXIT_FAILURE);
  i = 0;
  while (environ[i] != NULL)
  {
    tmp = ft_strdup(environ[i]);
    if (!tmp)
      exit(EXIT_FAILURE);
    list_env = ft_lstnew(tmp);
    if (!list_env)
      exit(EXIT_FAILURE);
    ft_lstadd_back(dup_envp, list_env);
    i++;
  }
}
void init_minishell(){
  env_to_list(&dup_envp);
}
void  sa_del(void *data)
{
  (void)data;
}

/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */
COMMAND *find_command (char *name)
{
  register int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp (name, commands[i].name) == 0)
      return (&commands[i]);

  return ((COMMAND *)NULL);
}

/* Execute a command line. */
int execute_line (line)
     char *line;
{
  register int i;
  COMMAND *command;
  char *word;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && is_whitespace(line[i]))
    i++;
  word = line + i;

  while (line[i] && !is_whitespace(line[i]))
    i++;

  if (line[i])
    line[i++] = '\0';

  command = find_command (word);

  if (!command)
    {
      fprintf (stderr, "%s: No such command.\n", word);
      return (-1);
    }

  /* Get argument to command, if any. */
  while (is_whitespace(line[i]))
    i++;

  word = line + i;

  /* Call the function. */
  return ((*(command->func)) (word));
}
int valid_argument (caller, arg)
     char *caller, *arg;
{
	char **userArgs = parseArgs(arg);
	if(userArgs == NULL || userArgs[0] == NULL || strcmp(caller, userArgs[0]) != 0)
		return (0);
	return (1);
}
char **parseArgs(char *args){
	if (args && *args){
		char **mc = ft_split(args, ' ');
		return mc;
	}
	return NULL;
}
int	main(int argc, char *argv[])
{
	if(argc){

	}

  	char *line, *s;

  	progname = argv[0];
    init_minishell();

  /* Loop reading and executing lines until the user quits. */
  for ( ; done == 0; )
    {
      line = readline ("minishell>");

      if (!line)
        break;

      s = ft_strtrim(line, " ");

      if (*s)
        {
          add_history (s);
          t_list *lexer;
          lexer = get_lexer(s);
          run_lexer(lexer);
        }
      free (line);
    }
	return 0;
}