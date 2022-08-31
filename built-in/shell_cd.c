#include "built_in.h"

/* change the current working directory */
int shell_cd(t_list *tokens){
	char *arg;
	int status;
	
	if (tokens == 0)
		arg = ".";
	else
		arg = tokens->content;
	status = chdir(arg);
	if (status<0)
	{
		ft_putstr_fd(ft_strjoin("Error changing directory", "\n"), STDIN_FILENO);
		return (status);
	}

	return (status);
}