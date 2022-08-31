#include "built_in.h"

/* Print out the current working directory. */
int shell_pwd (char *arg)
{
	if (!arg)
    	arg = "";
  	char dir[1024], *s;

  	s = getcwd (dir, sizeof(dir) - 1);
  	if (s == 0){
      	printf ("Error getting pwd: %s\n", dir);
      	return 1;
    }

  	ft_putstr_fd(ft_strjoin(dir, "\n"), STDIN_FILENO);
  	return 0;
}