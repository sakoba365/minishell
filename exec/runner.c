#include "runner.h"

int	**init_pipes(int numberOfCommand)
{
	int	**pipes;
	int	i;

	if (numberOfCommand == 1)
		return (0);
	pipes = ft_calloc(numberOfCommand, sizeof(int *));
	if (!pipes)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < numberOfCommand - 1)
	{
		pipes[i] = ft_calloc(2, sizeof(int));
		if (!pipes[i])
			exit(EXIT_FAILURE);
		if (pipe(pipes[i]) == -1)
			exit(EXIT_FAILURE);
		i++;
	}
	return (pipes);
}

void	run_cmd_table(t_cmd_table *cmd_table)
{
	t_list	*cmds;
	t_list	*pids;
	int		i;
	pid_t pid;
	int		exit_info;

	exit_info = 0;

	cmd_table->return_value = -1;
	cmds = cmd_table->cmds;
	cmd_table->nb_cmds = ft_lstsize(cmds);
	cmd_table->pipes = init_pipes(cmd_table->nb_cmds);
	i = 0;
	while (i < cmd_table->nb_cmds)
	{
		exec_cmd(cmds->content, cmd_table, i);
		cmds = cmds->next;
		i++;
	}
	dispose(cmd_table->pipes, cmd_table->nb_cmds);
	// run parent commad
	pids = cmd_table->pids;
	while(pids){
		pid = (pid_t)(*pids).content;
		waitpid(pid, &exit_info, WUNTRACED);
		if (WIFEXITED(exit_info))
			exit_status = WEXITSTATUS(exit_info);
		else if (WIFSIGNALED(exit_info))
			exit_status = WTERMSIG(exit_info);
		ft_lstdelone(&pids, sa_del);
	}
	release_array((void **)cmd_table->pipes);
	if (cmd_table->return_value != -1)
		exit_status = cmd_table->return_value;
}

void	dispose(int **pipes, int numberOfCommands)
{
	int	i;
	int	nb_pipes;

	i = 0;
	nb_pipes = numberOfCommands - 1;
	while (i < nb_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
void	exec_cmd(t_cmd *cmd, t_cmd_table *cmd_table, int process_index)
{
	(void)process_index;
	int	saved_stdin;
	int	saved_stdout;
	int	nb_cmds;
	t_list **dup_envp;
	dup_envp = NULL;
	COMMAND *command;


	nb_cmds = cmd_table->nb_cmds;
	filt_envs(&cmd->tokens);
	if(1<0)
		return;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	set_redirs_pipes(cmd->redirs, cmd_table, process_index);
	if (cmd->tokens != 0)
	{
		
		command = find_command (cmd->tokens->content);
		if (command){
			((*(command->func)) (cmd->tokens->next));
		}
		else{
			run_program(cmd->tokens, cmd_table);
		}
		
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
void run_lexer(t_list *lexer){
	char	*delimiter;
	t_cmd_table		*curr_lexer;

	while(lexer){
		curr_lexer = lexer->content;
		run_cmd_table(curr_lexer);
		delimiter = curr_lexer->delimiter;
		if(delimiter == NULL){
			break;
		}
		if (!ft_strncmp(delimiter, "&&", 2) && exit_status != EXIT_SUCCESS){
			break ;
		}
		if (!ft_strncmp(delimiter, "||", 2) && exit_status != EXIT_SUCCESS){
			break ;
		}
		lexer = lexer->next;
	}	
}
void	run_program(t_list *lst_tokens, t_cmd_table *cmd_table)
{
	char	**tokens;
	long	pid;
	t_list	*new_node;

	tokens = list_to_array(lst_tokens);
	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
		exec_background(tokens, cmd_table->nb_cmds, cmd_table->pipes);
	new_node = ft_lstnew((void *)pid);
	if (!new_node)
		exit(1);
	ft_lstadd_back(&cmd_table->pids, new_node);
	free(tokens);
}
void release_array(void **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
		free(ptr[i++]);
	free(ptr);
}

void	exec_background(char **tokens, int numberOfCommands, int **pipes)
{
	char	*exec_path;
	struct stat	statbuf;

	signal(SIGINT, SIG_DFL);
	dispose(pipes, numberOfCommands);

	if (stat(*tokens, &statbuf) == EXIT_SUCCESS){
		exec_path = ft_strdup(tokens[0]);
		if (!exec_path)
			exit(1);
	}else{
		exec_path = get_absolute_path(tokens[0]);
	}
	execve(exec_path, tokens, environ);
	if (errno == EACCES)
		printf("%s: Permission denied", tokens[0]);
	else if (errno == ENOENT && ft_strncmp(tokens[0], "exit", 4) != 0)
		printf("%s:command not found %d\n", tokens[0], errno);
	free(exec_path);
	free(tokens);
	if (errno == EACCES)
		exit(1);
	else if (errno == ENOENT)
		exit(1);
	exit(errno);
}
char	*get_absolute_path(char *program_name)
{
	char	**path_env_split;
	char	*absolute_path;

	path_env_split = get_path_env();
	if (!path_env_split)
	{
		absolute_path = ft_strdup(program_name);
		if (!absolute_path)
			exit(1);
	}
	else
	{
		absolute_path = get_programme_path(program_name, path_env_split);
		release_array((void **)path_env_split);
	}
	return (absolute_path);
}
char	**get_path_env(void)
{
	char	*path_env;
	char	**path_env_split;

	path_env = getenv("PATH");
	if (!path_env)
		return (0);
	path_env_split = ft_split(path_env, ':');
	if (!path_env_split)
		exit(1);
	return (path_env_split);
}

char	*get_programme_path(char *program_name, char **path_env_split)
{
	char		*absolute_path;
	int			i;
	struct stat	statbuf;

	i = 0;
	while (path_env_split[i])
	{
		absolute_path = ft_strjoin(ft_strjoin(path_env_split[i], "/"), program_name);
		if (!absolute_path)
			exit(1);
		if (stat(absolute_path, &statbuf) == EXIT_SUCCESS)
			break ;
		free(absolute_path);
		i++;
	}
	if (!path_env_split[i])
	{
		absolute_path = ft_strdup(program_name);
		if (!absolute_path)
			exit(1);
	}
	return (absolute_path);
}
char	**list_to_array(t_list *lst)
{
	int		i;
	int		len;
	char	**value;

	i = 0;
	len = ft_lstsize(lst);
	value = ft_calloc(len + 1, sizeof(char *));
	while (i < len)
	{
		value[i++] = (char *)lst->content;
		lst = lst->next;
	}
	return value;
}