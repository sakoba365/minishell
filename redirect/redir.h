#include "../minishell.h"

int	open_all_files(t_list *redirs);
int	write_file(t_redir *redir, int prev_fd, int flags, mode_t permissions);
int	has_redirs(t_list *redirs, char *type);