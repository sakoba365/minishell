#include "built_in.h"

/* Print out a list of the current environment variables. */
int shell_env(char *arg)
{
	if (!arg)
    	arg = "";
	char **envpl = environ;
	int i;
	for(i=0; envpl[i] !=NULL; i++)
		ft_putstr_fd(ft_strjoin(envpl[i], "\n"), STDIN_FILENO);

	return 0;
}