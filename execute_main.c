#include "header.h"

int	execute_main(t_main_data *main_data)
{
	int	i;
	int	status;

	i = 0;
	while (more_argv_left(main_data))
	{
		create_cmd(main_data);
		skip_pipe(main_data);
	}
	while (i < main_data->num_proc)
	{
		waitpid((main_data->proc)[i], &status, 0);
		printf("waited for %d\n", (main_data->proc)[i]);
		i++;
	}
	// wait for all cmds
	// get exit code of last one
	return (0);
}

int	create_cmd(t_main_data *main_data)
{
	t_cmd	cmd;
	int		i;

	cmd.argv[0] = main_data->cur_argv;
	cmd.argv[1] = NULL;
	cmd.name = main_data->cur_argv;
	go_to_next_argv(main_data);
	main_data->more_cmd_left = false;
	printf("cmd name: %s\n", cmd.name);
	i = 1;
	while (more_argv_left(main_data) && !cur_argv_is_pipe(main_data))
	{
		cmd.argv[i] = main_data->cur_argv;
		i++;
		cmd.argv[i] = NULL;
		printf("cmd argv: %s\n", cmd.argv[i - 1]);
		go_to_next_argv(main_data);
	}
	if (more_argv_left(main_data))
		main_data->more_cmd_left = true;
	execute_cmd(main_data, cmd);
	printf("\n\n\n");
	return (0);
}

// if (dup2(fd_in, STDIN_FILENO) == -1)
// 		clean_exit_error(data, 1, "");
// 	if (dup2(data->fd_pipe[1], STDOUT_FILENO) == -1)
// 		clean_exit_error(data, 1, "");

int	execute_cmd(t_main_data *main_data, t_cmd cmd)
{
	(main_data->proc)[main_data->num_proc] = fork();
	if (main_data->proc[main_data->num_proc] == -1)
		free_exit(1, main_data);
	// printf("pid %d\n", (main_data->proc)[main_data->num_proc]);
	if (pipe((main_data->pipes)[main_data->num_proc]))
		free_exit(1, main_data);
	if (main_data->proc[main_data->num_proc] == 0)
	{
		child_execute(main_data, &cmd);
	}
	else
	{
		printf("num %d\n", main_data->num_proc);
		main_data->num_proc++;
	}
	return (0);
}

int	child_execute(t_main_data *main_data, t_cmd *cmd)
{
	char *argv[2] = {"ls", NULL};

	if (main_data->num_proc > 0)
		redirect_in(main_data);
	if (main_data->more_cmd_left)
		redirect_out(main_data);
	printf("execing\n");
	execve("ls", argv, main_data->envp);
	// execve(cmd->name, cmd->argv, main_data->envp);
	printf("exec gone wrong\n");
	exit(1);
	return (0);
}

int	redirect_in(t_main_data *main_data)
{
	if (dup2((main_data->pipes)[main_data->num_proc - 1][0], STDIN_FILENO) ==
		-1)
		free_exit(1, main_data);
	return (0);
}

int	redirect_out(t_main_data *main_data)
{
	if (dup2((main_data->pipes)[main_data->num_proc][1], STDOUT_FILENO) == -1)
		free_exit(1, main_data);
	return (0);
}
