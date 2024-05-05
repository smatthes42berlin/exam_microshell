#include "header.h"

// int	main(void)
// {
// 	return (0);
// }

// ./microshell /bin/ls | wc -l ; echo hi | wc; echo hi

int	main(int argc, char *argv[], char *envp[])
{
	t_main_data	main_data;

	init_data(&main_data, argc, argv, envp);
	if (argc <= 1)
		return (0);
	parse_data(&main_data);
	print(&main_data);
	// execute();
}

void	init_data(t_main_data *main_data, int argc, char **argv, char **envp)
{
	main_data->envp = envp;
	main_data->argv = argv;
	main_data->argc = argc;
	main_data->cmd_seq = NULL;
	main_data->cur_seq = NULL;
	main_data->cur_cmd = NULL;
	main_data->cur_argv = 1;
}
