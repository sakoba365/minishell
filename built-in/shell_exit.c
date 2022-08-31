#include "built_in.h"


int shell_exit(char *arg){
	if (!arg)
    	arg = "";
  	done = 1;
  	return (0);
}