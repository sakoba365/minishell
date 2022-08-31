#include "built_in.h"

/* Print out all exported variables. */
int shell_export(char *arg)
{
	if (!arg)
    	arg = "";
    char **envpl = environ;
   	int x, i, j, n;
   	char envp_data[1024][1024], s[1024];
	for(x=0; envpl[x] !=NULL; x++){
		strcpy(envp_data[x], envpl[x]);
	}
   n = x;
   for(i=0;i<n;i++){
      for(j=i+1;j<n;j++){
         if(strcmp(envp_data[i],envp_data[j])>0){
            strcpy(s,envp_data[i]);
            strcpy(envp_data[i],envp_data[j]);
            strcpy(envp_data[j],s);
         }
      }
   }
   for(i=0;i<n;i++){
   		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		char **mc = ft_split(envp_data[i], '=');
		if(mc){
			if(mc[0] != NULL)
				ft_putstr_fd(ft_strjoin(mc[0],"=\""), STDIN_FILENO);
			if(mc[1] != NULL)
				ft_putstr_fd(ft_strjoin(mc[1],"\"\n"), STDIN_FILENO);
			else
				ft_putstr_fd("\"\n", STDIN_FILENO);
		}else{
			ft_putstr_fd(ft_strjoin(envp_data[i], "\n"), STDIN_FILENO);
		}
	}
	return 0;
}