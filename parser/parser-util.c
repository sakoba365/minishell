#include "../minishell.h"


void	skip_spaces(const char *input, int *curr_pos)
{
	while (is_whitespace(input[*curr_pos]))
		(*curr_pos)++;
}

char	*get_cmd_table_delimiter(const char *input, int *curr_pos)
{
	char	*delimiter;

	delimiter = 0;
	if (input[*curr_pos] == '\0')
		delimiter = ft_strdup("");
	else if (input[*curr_pos] == ';')
		delimiter = ft_strdup(";");
	else if (!ft_strncmp((char *)&input[*curr_pos], "||", 2))
		delimiter = ft_strdup("||");
	else if (!ft_strncmp((char *)&input[*curr_pos], "&&", 2))
		delimiter = ft_strdup("&&");
	if (!delimiter)
		exit(1);
	(*curr_pos) += ft_strlen(delimiter);
	return (delimiter);
}
int is_token_delimiter(char c){
	int	check;

	if (ft_strchr(WHITE_SPACE, c) != 0)
		check = 1;
	else if (c == ';')
		check = 1;
	else if (c == '|')
		check = 1;
	else if (c == '&')
		check = 1;
	else if (c == '<')
		check = 1;
	else if (c == '>')
		check = 1;
	else
		check = 0;
	return (check);
}
void	skip_quotes(const char *str, int *i)
{
	int		has_double_quotes_open;
	int		has_single_quotes_open;

	has_double_quotes_open = 0;
	has_single_quotes_open = 0;
	while (str[*i])
	{
		if (str[*i] == '"' && has_single_quotes_open == 0)
			has_double_quotes_open = !has_double_quotes_open;
		else if (str[*i] == '\'' && has_double_quotes_open == 0)
			has_single_quotes_open = !has_single_quotes_open;
		else if (!has_double_quotes_open && !has_single_quotes_open)
			break ;
		(*i)++;
	}
}
