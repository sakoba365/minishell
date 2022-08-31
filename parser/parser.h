#include "../minishell.h"


char	*get_token(const char *input, int *curr_pos);
t_cmd	*get_cmd(const char *input, int *curr_pos);
t_redir	*get_redir(const char *input, int *curr_pos);
t_cmd_table	*get_cmd_table(const char *input, int *curr_pos);
t_ast	*get_ast(const char *input);
void	skip_spaces(const char *input, int *curr_pos);
char	*get_cmd_table_delimiter(const char *input, int *curr_pos);
void	exec_cmd(t_cmd *cmd, t_cmd_table *cmd_table, int process_index);
void	exec_cmd_table(t_cmd_table *cmd_table);
int is_token_delimiter(char c);
void	skip_quotes(const char *str, int *i);
