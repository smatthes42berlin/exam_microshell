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
	dprintf(STDERR_FILENO,"main : \t started all cmds\n");
	fflush(stdout);
	fflush(stderr);
	dprintf(STDERR_FILENO,"main : \t num processes created %d\n", main_data->num_proc);
	close_pipe(main_data->pipe_left);
	close_pipe(main_data->pipe_right);
	// close_all_pipes(main_data, main_data->num_proc);
	while (i < main_data->num_proc)
	{
		dprintf(STDERR_FILENO,"main : \t waiting for %d \n", (main_data->proc)[i]);
		waitpid((main_data->proc)[i], &status, 0);
		dprintf(STDERR_FILENO,"main : \t waited for %d\n", (main_data->proc)[i]);
		i++;
	}
	// wait for all cmds
	// get exit code of last one
	return (0);
}

int	close_all_pipes(t_main_data *main_data, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		dprintf(STDERR_FILENO,"closing pipe i = %d\n", i);
		close(main_data->pipes[i][0]);
		close(main_data->pipes[i][1]);
		i++;
	}
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
	dprintf(STDERR_FILENO,"cmd name: %s\n", cmd.name);
	i = 1;
	while (more_argv_left(main_data) && !cur_argv_is_pipe(main_data))
	{
		cmd.argv[i] = main_data->cur_argv;
		i++;
		cmd.argv[i] = NULL;
		dprintf(STDERR_FILENO,"cmd argv: %s\n", cmd.argv[i - 1]);
		go_to_next_argv(main_data);
	}
	if (more_argv_left(main_data))
		main_data->more_cmd_left = true;
	execute_cmd(main_data, cmd);
	dprintf(STDERR_FILENO,"\n\n\n");
	return (0);
}

int	close_pipe(int fd[2])
{
	close(fd[1]);
	close(fd[0]);
	return (0);
}

int	assign_pipe(int fd_from[2], int fd_to[2])
{
	fd_to[0] = fd_from[0];
	fd_to[1] = fd_from[1];
	return (0);
}

int	execute_cmd(t_main_data *main_data, t_cmd cmd)
{
	assign_pipe(main_data->pipe_left, main_data->pipe_close);
	assign_pipe(main_data->pipe_right, main_data->pipe_left);
	pipe(main_data->pipe_right);
	close_pipe(main_data->pipe_close);
	(main_data->proc)[main_data->num_proc] = fork();
	if (main_data->proc[main_data->num_proc] == -1)
		free_exit(1, main_data);
	// if (pipe((main_data->pipes)[main_data->num_proc]))
	// 	free_exit(1, main_data);
	if (main_data->proc[main_data->num_proc] == 0)
	{
		child_execute(main_data, &cmd);
	}
	else
	{
		// list_open_fd(NULL);
		dprintf(STDERR_FILENO,"main : \t  created %s %d\n", cmd.name,
				(main_data->proc)[main_data->num_proc]);
		dprintf(STDERR_FILENO,"main : \t  num %d\n", main_data->num_proc);
		main_data->num_proc++;
	}
	return (0);
}

int	child_execute(t_main_data *main_data, t_cmd *cmd)
{
	dprintf(STDERR_FILENO,"%s : \t execcing start\n", cmd->name);
	if (main_data->num_proc > 0)
		redirect_in(main_data, cmd);
	if (main_data->more_cmd_left)
		redirect_out(main_data, cmd);
	close_pipe(main_data->pipe_left);
	close_pipe(main_data->pipe_right);
	dprintf(STDERR_FILENO,"%s : \t execcing \n", cmd->name);
	// if (main_data->num_proc || true)
	// {
	// 	printf("closing all pipes for %s\n", cmd->name);
	// 	close_all_pipes(main_data, main_data->num_proc + 1);
	// }
	list_open_fd(cmd);
	dprintf(STDERR_FILENO,"%s : \t passing everything to execve\n", cmd->name);
	fflush(stdout);
	fflush(stderr);
	execve(cmd->name, cmd->argv, main_data->envp);
	dprintf(STDERR_FILENO,"ERROR exec gone wrong\n");
	exit(1);
	return (0);
}

int	redirect_in(t_main_data *main_data, t_cmd *cmd)
{
	dprintf(STDERR_FILENO,"\n\n\n%s : \t entering redirect in\n", cmd->name);
	if (dup2((main_data->pipe_left)[0], STDIN_FILENO) ==
		-1)
	{
		dprintf(2, "error redirecting in\n");
		free_exit(1, main_data);
	}
	dprintf(STDERR_FILENO,"%s : \t leaving redirect in\n\n", cmd->name);
	return (0);
}

int	redirect_out(t_main_data *main_data, t_cmd *cmd)
{
	dprintf(STDERR_FILENO,"\n\n\n%s : \t entering redirect out\n", cmd->name);
	if (dup2((main_data->pipe_right)[1], STDOUT_FILENO) ==
		-1)
	{
		dprintf(2, "error redirecting out\n");
		free_exit(1, main_data);
	}
	dprintf(2, "%s : \t leaving redirect out\n\n", cmd->name);
	return (0);
}

// int	redirect_in(t_main_data *main_data, t_cmd *cmd)
// {
// 	printf("\n\n\nentering redirect in %s\n", cmd->name);
// 	printf("redirecting STDIN_FILENO to %d\n",
// 			(main_data->pipes)[main_data->num_proc - 1][0]);
// 	if (dup2((main_data->pipes)[main_data->num_proc - 1][0], STDIN_FILENO) ==
// 		-1)
// 	{
// 		dprintf(2, "error redirecting in\n");
// 		free_exit(1, main_data);
// 	}
// 	printf("leaving redirect in %s\n\n", cmd->name);
// 	return (0);
// }

// int	redirect_out(t_main_data *main_data, t_cmd *cmd)
// {
// 	printf("\n\n\nentering redirect out %s\n", cmd->name);
// 	printf("redirecting STDOUT_FILENO to %d\n",
// 			(main_data->pipes)[main_data->num_proc][1]);
// 	printf("before dup2\n");
// 	// list_open_fd(cmd);
// 	if (dup2((main_data->pipes)[main_data->num_proc][1], STDOUT_FILENO) ==
// 		-1)
// 	{
// 		dprintf(2, "error redirecting out\n");
// 		free_exit(1, main_data);
// 	}
// 	dprintf(2, "after dup2\n");
// 	dprintf(2, "leaving redirect out %s\n\n", cmd->name);
// 	return (0);
// }
