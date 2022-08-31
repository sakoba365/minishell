#include "../minishell.h"

void run_cmd_table(t_cmd_table *cmd_table);
int	**init_pipes(int nb_cmds);
void dispose(int **pipes, int numberOfCommands);
void	exec_cmd(t_cmd *cmd, t_cmd_table *cmd_table, int process_index);
void	run_program(t_list *lst_tokens, t_cmd_table *cmd_table);


void	exec_background(char **tokens, int numberOfCommands, int **pipes);
char	*get_absolute_path(char *program_name);
char	**get_path_env(void);
char	*get_programme_path(char *program_name, char **path_env_split);
char	**list_to_array(t_list *lst);
