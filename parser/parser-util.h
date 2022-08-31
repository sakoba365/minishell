#include "../minishell.h"
void	skip_spaces(const char *input, int *curr_pos);
char	*get_cmd_table_delimiter(const char *input, int *curr_pos);
int 	is_token_delimiter(char c);
void	skip_quotes(const char *str, int *i);
