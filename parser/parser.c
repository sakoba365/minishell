#include "parser.h"


t_list *get_lexer(const char *input){
	t_list	*cmd_table;
	int		curr_pos;	


	curr_pos = 0;
	while (input[curr_pos])
	{
		skip_spaces(input, &curr_pos);
		cmd_table = ft_lstnew((void *)get_cmd_table(input, &curr_pos));
		if (!cmd_table)
			exit(EXIT_FAILURE);
		ft_lstadd_back(&cmd_table, cmd_table);
	}
	return (cmd_table);
}
char	*get_token(const char *input, int *curr_pos)
{
	char	*token;
	int		saved_pos;

	saved_pos = *curr_pos;
	while (input[*curr_pos] && !is_token_delimiter(input[*curr_pos]))
	{
		if (is_quote(input[*curr_pos]))
			skip_quotes(input, curr_pos);
		else if (input[*curr_pos])
			(*curr_pos)++;
	}
	token = ft_substr(input, saved_pos, *curr_pos - saved_pos);
	if (!token)
		return "";
	return (token);
}

t_cmd	*get_cmd(const char *input, int *curr_pos)
{
	t_cmd	*cmd;
	t_list	*new_node;
	t_redir	*redir;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		exit(EXIT_FAILURE);
	while (input[*curr_pos] && !is_delim_cmd(input[*curr_pos]))
	{
		if (!is_redirect(input[*curr_pos]))
		{
			new_node = ft_lstnew((void *)get_token(input, curr_pos));
			if (!new_node)
				exit(EXIT_FAILURE);
			ft_lstadd_back(&cmd->tokens, new_node);
		}
		else
		{
			redir = ft_calloc(1, sizeof(t_redir));
			if (redir){
				if (!ft_strncmp(&input[*curr_pos], ">>", 2))
				{
					ft_strlcpy(redir->type, (char *)&input[*curr_pos], 2);
					*curr_pos += 2;
				}
				else if (input[*curr_pos] == '<' || input[*curr_pos] == '>')
					*redir->type = input[(*curr_pos)++];
				skip_spaces(input, curr_pos);
				redir->direction = get_token(input, curr_pos);
				new_node = ft_lstnew((void *)redir);
				if (!new_node)
					exit(EXIT_FAILURE);
				ft_lstadd_back(&cmd->redirs, new_node);
			}
		}
		skip_spaces(input, curr_pos);
	}
	return (cmd);
}
t_cmd_table	*get_cmd_table(const char *input, int *curr_pos)
{
	t_cmd_table	*cmd_table;
	t_list		*cmd;

	cmd_table = ft_calloc(1, sizeof(t_cmd_table));
	if (!cmd_table)
		exit(EXIT_FAILURE);
	while (input[*curr_pos])
	{
		if(input[*curr_pos] ==';' || !ft_strncmp(&input[*curr_pos], "||", 2) || !ft_strncmp(&input[*curr_pos], "&&", 2)){
			cmd_table->delimiter = get_cmd_table_delimiter(input, curr_pos);
			break;
		}
		skip_spaces(input, curr_pos);
		cmd = ft_lstnew((void *)get_cmd(input, curr_pos));
		if (!cmd)
			exit(EXIT_FAILURE);
		ft_lstadd_back(&cmd_table->cmds, cmd);
		if (input[*curr_pos] == '|' && input[*curr_pos + 1] != '|')
			(*curr_pos)++;
	}
	
	return (cmd_table);
}