#include "built_in.h"


int shell_echo (t_list *tokens){
	char	*arg;
	int		line_break;
	int has_value;

	line_break = 1;
	has_value = 0;
	while (tokens)
	{
		arg = tokens->content;
		if (strcmp(arg, "-n") == 0) {
			line_break = 0;
			
		}
		else{
			has_value = 1;
	  		ft_putstr_fd(arg, STDOUT_FILENO);
	  		ft_putchar_fd(' ', STDOUT_FILENO);
	  	}
	  	tokens = tokens->next;
  	}
  	if(line_break && has_value)
  		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}