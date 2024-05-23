/* header */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROC_LIMIT 1000
#define ARGV_LIMIT 1000

typedef struct s_cmd
{
	char	*name;
	char	*argv[ARGV_LIMIT];
}			t_cmd;

typedef struct s_main_data
{
	int		num_cur_argv;
	char	*cur_argv;
	t_cmd	*cur_cmd;
	int		proc[PROC_LIMIT];
	int		num_proc;
	int		pipe_left[2];
	int		pipe_right[2];
	int		pipe_close[2];
	bool	more_cmd_left;
	char	**envp;
	char	**argv;
	int		argc;
}			t_main_data;

/* init */

void		init_cmd_vars(t_main_data *main_data);
void		init_main_args(t_main_data *main_data, int argc, char **argv,
				char **envp);

/* execute main */

int			execute_main(t_main_data *main_data);
int			create_cmd(t_main_data *main_data);
int			execute_cmd(t_main_data *main_data);
int			child_execute(t_main_data *main_data);
int			redirect_in(t_main_data *main_data);
int			redirect_out(t_main_data *main_data);
int			close_all_pipes(t_main_data *main_data, int num);
int			get_all_args(t_main_data *main_data);

/* cd */

bool		is_cd(t_main_data *main_data);
int			handle_cd(t_main_data *main_data);

// util

void		skip_semicolon(t_main_data *main_data);
void		skip_pipe(t_main_data *main_data);
bool		is_semicolon(char *str);
bool		is_pipe(char *str);
bool		cur_argv_is_semicolon(t_main_data *main_data);
bool		cur_argv_is_pipe(t_main_data *main_data);
char		*get_cur_argv(t_main_data *main_data);
void		go_to_next_argv(t_main_data *main_data);
bool		more_argv_left(t_main_data *main_data);
int			find_next_pipe_or_semi_or_end(t_main_data *main_data);
int			list_open_fd(t_cmd *cmd);
int			close_pipe(int fd[2]);
int			assign_pipe(int fd_from[2], int fd_to[2]);
void		print(char *str);
int			error_sys_call_exit(int code);

/* main */

int	main(int argc, char *argv[], char *envp[])
{
	t_main_data	main_data;
	int			res;

	res = 0;
	init_main_args(&main_data, argc, argv, envp);
	if (argc <= 1)
		return (0);
	while (more_argv_left(&main_data))
	{
		skip_semicolon(&main_data);
		if (!more_argv_left(&main_data))
			break ;
		if (is_cd(&main_data))
			res = handle_cd(&main_data);
		else
		{
			init_cmd_vars(&main_data);
			res = execute_main(&main_data);
		}
	}
	return (res);
}

void	init_cmd_vars(t_main_data *main_data)
{
	main_data->num_proc = 0;
	main_data->more_cmd_left = false;
	if (pipe(main_data->pipe_left) == -1)
		error_sys_call_exit(1);
	if (pipe(main_data->pipe_right) == -1)
		error_sys_call_exit(1);
}

void	init_main_args(t_main_data *main_data, int argc, char **argv,
		char **envp)
{
	main_data->envp = envp;
	main_data->argv = argv;
	main_data->argc = argc;
	main_data->num_cur_argv = 1;
	main_data->cur_argv = (main_data->argv)[1];
}

/* cd */

bool	is_cd(t_main_data *main_data)
{
	return (strcmp("cd", main_data->cur_argv) == 0);
}

int	handle_cd(t_main_data *main_data)
{
	t_cmd	cmd;
	int		num_args;

	go_to_next_argv(main_data);
	main_data->cur_cmd = &cmd;
	num_args = get_all_args(main_data);
	if (num_args != 1)
	{
		print("error: cd: bad arguments\n");
		return (1);
	}
	if (chdir(cmd.argv[1]) == -1)
	{
		print("error: cd: cannot change directory to ");
		print(cmd.argv[1]);
		print("\n");
		return (1);
	}
	return (0);
}

/* execute */

int	execute_main(t_main_data *main_data)
{
	int	i;
	int	status;
	int	res_wait;

	i = 0;
	while (more_argv_left(main_data) && !cur_argv_is_semicolon(main_data))
	{
		create_cmd(main_data);
		skip_pipe(main_data);
	}
	close_pipe(main_data->pipe_left);
	close_pipe(main_data->pipe_right);
	while (i < main_data->num_proc)
	{
		res_wait = waitpid((main_data->proc)[i], &status, 0);
		if (res_wait == -1)
			error_sys_call_exit(1);
		i++;
	}
	if (WIFEXITED(status))
		res_wait = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		res_wait = WTERMSIG(status);
	return (res_wait);
}

int	create_cmd(t_main_data *main_data)
{
	t_cmd	cmd;

	cmd.argv[0] = main_data->cur_argv;
	cmd.argv[1] = NULL;
	cmd.name = main_data->cur_argv;
	main_data->cur_cmd = &cmd;
	go_to_next_argv(main_data);
	main_data->more_cmd_left = false;
	get_all_args(main_data);
	execute_cmd(main_data);
	return (0);
}

int	get_all_args(t_main_data *main_data)
{
	t_cmd	*cmd;
	int		i;

	i = 1;
	cmd = main_data->cur_cmd;
	while (more_argv_left(main_data) && !cur_argv_is_pipe(main_data)
		&& !cur_argv_is_semicolon(main_data))
	{
		(cmd->argv)[i] = main_data->cur_argv;
		i++;
		(cmd->argv)[i] = NULL;
		go_to_next_argv(main_data);
	}
	if (more_argv_left(main_data) && cur_argv_is_pipe(main_data))
		main_data->more_cmd_left = true;
	return (i - 1);
}

int	execute_cmd(t_main_data *main_data)
{
	assign_pipe(main_data->pipe_left, main_data->pipe_close);
	assign_pipe(main_data->pipe_right, main_data->pipe_left);
	pipe(main_data->pipe_right);
	close_pipe(main_data->pipe_close);
	(main_data->proc)[main_data->num_proc] = fork();
	if (main_data->proc[main_data->num_proc] == -1)
		error_sys_call_exit(1);
	if (main_data->proc[main_data->num_proc] == 0)
		child_execute(main_data);
	else
		main_data->num_proc++;
	return (0);
}

int	child_execute(t_main_data *main_data)
{
	t_cmd	*cmd;

	cmd = main_data->cur_cmd;
	if (main_data->num_proc > 0)
		redirect_in(main_data);
	if (main_data->more_cmd_left)
		redirect_out(main_data);
	close_pipe(main_data->pipe_left);
	close_pipe(main_data->pipe_right);
	execve(cmd->name, cmd->argv, main_data->envp);
	print("error: cannot execute ");
	print(cmd->name);
	print("\n");
	exit(1);
	return (0);
}

int	redirect_in(t_main_data *main_data)
{
	if (dup2((main_data->pipe_left)[0], STDIN_FILENO) ==
		-1)
		error_sys_call_exit(1);
	return (0);
}

int	redirect_out(t_main_data *main_data)
{
	if (dup2((main_data->pipe_right)[1], STDOUT_FILENO) ==
		-1)
		error_sys_call_exit(1);
	return (0);
}

/* utils */

bool	more_argv_left(t_main_data *main_data)
{
	return (main_data->num_cur_argv < main_data->argc);
}

bool	cur_argv_is_pipe(t_main_data *main_data)
{
	return (is_pipe(main_data->cur_argv));
}

bool	cur_argv_is_semicolon(t_main_data *main_data)
{
	return (is_semicolon(main_data->cur_argv));
}

bool	is_pipe(char *str)
{
	return (!strcmp(str, "|"));
}

bool	is_semicolon(char *str)
{
	return (!strcmp(str, ";"));
}

void	go_to_next_argv(t_main_data *main_data)
{
	(main_data->num_cur_argv)++;
	main_data->cur_argv = (main_data->argv)[main_data->num_cur_argv];
}

void	skip_pipe(t_main_data *main_data)
{
	while (more_argv_left(main_data) && cur_argv_is_pipe(main_data))
	{
		go_to_next_argv(main_data);
	}
}

void	skip_semicolon(t_main_data *main_data)
{
	while (more_argv_left(main_data) && cur_argv_is_semicolon(main_data))
	{
		go_to_next_argv(main_data);
	}
}

int	close_pipe(int fd[2])
{
	if (close(fd[1]) == -1)
		error_sys_call_exit(1);
	if (close(fd[0]) == -1)
		error_sys_call_exit(1);
	return (0);
}

int	assign_pipe(int fd_from[2], int fd_to[2])
{
	fd_to[0] = fd_from[0];
	fd_to[1] = fd_from[1];
	return (0);
}

void	print(char *str)
{
	int	str_len;

	str_len = 0;
	while (str[str_len])
		str_len++;
	if (write(STDERR_FILENO, str, str_len) == -1)
		error_sys_call_exit(1);
}

int	error_sys_call_exit(int code)
{
	print("error: fatal\n");
	exit(code);
}
