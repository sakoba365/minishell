#include "redir.h"

void	set_redirs_pipes(t_list *redirs,
						t_cmd_table *cmd_table,
						int process_index)
{
	int	**pipes;
	int	nb_cmds;

	pipes = cmd_table->pipes;
	nb_cmds = cmd_table->nb_cmds;
	if (open_all_files(redirs) == EXIT_FAILURE)
		return ;
	if (!has_redirs(redirs, "<") && process_index != 0)
		dup2(pipes[process_index - 1][0], STDIN_FILENO);
	if (!has_redirs(redirs, ">") && !has_redirs(redirs, ">>")
		&& process_index != nb_cmds - 1)
		dup2(pipes[process_index][1], STDOUT_FILENO);
	exit_status = EXIT_SUCCESS;
}
int	open_all_files(t_list *redirs)
{
	int		fd_i;
	int		fd_o;
	t_redir	*redir;

	fd_i = -2;
	fd_o = -2;
	while (redirs)
	{
		redir = (t_redir *)redirs->content;
		if (!ft_strncmp(redir->type, "<", 1))
			fd_i = write_file(redir, fd_i, O_RDONLY, 0);
		else if (!ft_strncmp(redir->type, ">", 1))
			fd_o = write_file(redir, fd_o, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		else if (!ft_strncmp(redir->type, ">>", 2))
			fd_o = write_file(redir, fd_o, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
		if (fd_i == -1 || fd_o == -1)
			return (EXIT_FAILURE);
		redirs = redirs->next;
	}
	return (EXIT_SUCCESS);
}

int	write_file(t_redir *redir, int prev_fd, int flags, mode_t permissions)
{
	char	*file_name;
	int		new_fd;

	if (prev_fd != -2)
		close(prev_fd);
	file_name = redir->direction;
	new_fd = open(file_name, flags, permissions);
	if (new_fd == -1)
	{
		exit_status = errno;
	}
	else
	{
		if (!ft_strncmp(redir->type, "<", 1))
			dup2(new_fd, STDIN_FILENO);
		else if (!ft_strncmp(redir->type, ">", 1) || !ft_strncmp(redir->type, ">>", 2))
			dup2(new_fd, STDOUT_FILENO);
		close(new_fd);
	}
	return (new_fd);
}
int	has_redirs(t_list *redirs, char *type)
{
	int		res;
	t_redir	*redir;

	res = 0;
	while (redirs)
	{
		redir = (t_redir *)redirs->content;
		if (!strcmp(redir->type, type))
		{
			res = 1;
			break ;
		}
		redirs = redirs->next;
	}
	return (res);
}