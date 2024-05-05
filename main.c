#include "header.h"

// start with one cmd without semicolons
// first argv is cmd name, anymore argv are arguments
// when pipe repeat process

int	main(int argc, char *argv[], char *envp[])
{
	t_main_data	main_data;

	init_data(&main_data, argc, argv, envp);
	if (argc <= 1)
		return (0);
	execute_main(&main_data);
}

void	init_data(t_main_data *main_data, int argc, char **argv, char **envp)
{
	int	i;

	main_data->envp = envp;
	main_data->argv = argv;
	main_data->argc = argc;
	main_data->num_cur_argv = 1;
	main_data->cur_argv = (main_data->argv)[1];
	main_data->num_proc = 0;
	main_data->more_cmd_left = false;
	i = 0;
	while (i < PIPE_LIMIT)
	{
		main_data->pipes[i][0] = STDIN_FILENO;
		main_data->pipes[i][1] = STDOUT_FILENO;
		i++;
	}
	i = 0;
	while (i < PROC_LIMIT)
	{
		main_data->proc[i] = DEFAULT_INT;
		i++;
	}
}
