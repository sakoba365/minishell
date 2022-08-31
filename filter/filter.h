#include "../minishell.h"

void replace_node(t_list *tokens, char *old_content, char *new_content);
char	*getenv_custom(char *key);
void	delete_quotes(char *token);
char *str_replace(char *str, char *rep, char *with) ;