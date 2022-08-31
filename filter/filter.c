#include "filter.h"

void	filt_envs(t_list **tokens)
{
	t_list	*tmp_tokens;
	t_list	*split_token;
	char *token;
	char	**pieces;
	char *env_var;
	char *new_val ;
	char *var;
	int i;

	tmp_tokens = *tokens;
	split_token = *tokens;
	while (tmp_tokens)
	{
		token = tmp_tokens->content;
		if(!is_single_quote(*token)){
			pieces = ft_split(token, ' ');
			i = 0;
			for(i=0; pieces[i] !=NULL; i++){
				if(is_single_quote(*pieces[i])){
					continue;
				}
				env_var = ft_strnstr(pieces[i], "$", ft_strlen(pieces[i]));
				if(env_var){
					delete_quotes(env_var);
					var = ft_substr(env_var, 1, ft_strlen(env_var));
					if(strcmp(var, "?") !=0)
						new_val = str_replace(token, pieces[i], getenv_custom(var));
					else
						new_val = str_replace(token, pieces[i], ft_itoa(exit_status));
					replace_node(split_token, token, new_val);
				}
			}
		}

		delete_quotes((char *)tmp_tokens->content);
		tmp_tokens = tmp_tokens->next;

	}
}

char	*getenv_custom(char *key)
{
	int		i;
	t_list	*envp;
	char	*curr_envp;
	char	*value;

	envp = dup_envp;
	while (envp)
	{
		i = 0;
		curr_envp = (char *)envp->content;
		while (key[i] && curr_envp[i] && (key[i] == curr_envp[i]))
			i++;
		if (!key[i] && (curr_envp[i] == '='
				|| curr_envp[i] == '\0'))
		{
			value = ft_strdup(&(curr_envp[i + 1]));
			if (!value)
				exit(EXIT_FAILURE);
			return (value);
		}
		envp = envp->next;
	}
	return (NULL);
}
void replace_node(t_list *tokens, char *old_content, char *new_content){

	while(tokens){
		if(strcmp(tokens->content, old_content) == 0){
			strcpy(tokens->content, new_content);
		}
		tokens = tokens->next;
	}
}
char	*ft_strcpy(char *dest, const char *src)
{
	char	*saved;

	saved = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (saved);
}
void	delete_quotes(char *token)
{
	int	i;
	int	has_double_quotes_open;
	int	has_single_quotes_open;


	i = 0;
	has_double_quotes_open = 0;
	has_single_quotes_open = 0;
	while (token[i])
	{
		if (token[i] == '"' && has_single_quotes_open == 0)
		{
			has_double_quotes_open = !has_double_quotes_open;
			ft_strcpy(&token[i], (const char *)&token[i + 1]);
			continue ;
		}
		else if (token[i] == '\'' && has_double_quotes_open == 0)
		{
			has_single_quotes_open = !has_single_quotes_open;
			ft_strcpy(&token[i], (const char *)&token[i + 1]);
			continue ;
		}
		i++;
	}
}
char *str_replace(char *str, char *rep, char *with) {
    char *result; 
    char *ins;    
    char *tmp;    
    int len_rep; 
    int len_with; 
    int len_front;
    int count;   

    if (!str || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; 
    if (!with)
        with = "";
    len_with = strlen(with);
    ins = str;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(str) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(str, rep);
        len_front = ins - str;
        tmp = strncpy(tmp, str, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        str += len_front + len_rep;
    }
    strcpy(tmp, str);
    return result;
}