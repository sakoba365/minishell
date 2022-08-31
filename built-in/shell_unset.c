#include "built_in.h"

/* Print out all exported variables. */
int shell_unset(char *arg)
{
	if (!arg)
    	arg = "";
    char **envpl = environ;
	int x;
	for(x=0; envpl[x] !=NULL; x++){
		char **mc = ft_split(envpl[x], '=');
		if(mc != NULL && mc[0] != NULL){
			free(mc[0]);
		}
	}

	return 0;

}