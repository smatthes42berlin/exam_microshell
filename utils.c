#include "header.h"

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

void	free_exit(int code, t_main_data *main_data)
{
	exit(code);
	main_data->cur_argv++;
}

int	find_next_pipe_or_semi_or_end(t_main_data *main_data)
{
	char	*cur_argv;
	int		next;
	int		cur_pos;

	next = 0;
	cur_pos = main_data->num_cur_argv;
	cur_argv = main_data->argv[cur_pos];
	while (cur_pos < main_data->argc && !is_semicolon(cur_argv)
		&& !is_pipe(cur_argv))
	{
		next++;
		cur_pos++;
		cur_argv = main_data->argv[cur_pos];
	}
	return (next);
}

int	list_open_fd(t_cmd *cmd)
{
	int	max_fd;

	max_fd = getdtablesize();
	if (cmd)
		dprintf(STDERR_FILENO, "%s : \t Open descritors\n", cmd->name);
	else
		dprintf(STDERR_FILENO, "main : \t Open descritors for main\n");
	for (int fd = 0; fd < max_fd; fd++)
	{
		if (fcntl(fd, F_GETFD) != -1)
		{
			if (cmd)
				dprintf(STDERR_FILENO, "%s : open fd: \t %d\n", cmd->name, fd);
			else
				dprintf(STDERR_FILENO, "main : open fd: \t %d \n", fd);
		}
	}
	return (0);
}

// void	char_arr_fill_null(char **arr, int num)
// {
// 	while (num > 0)
// 	{
// 		arr[num] = NULL;
// 		num--;
// 	}
// }
