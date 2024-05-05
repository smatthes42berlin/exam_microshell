#include "header.h"

bool	is_semicolon(char *str)
{
	return (!strcmp(str, ";"));
}

bool	is_pipe(char *str)
{
	return (!strcmp(str, "|"));
}

bool	cur_argv_is_semicolon(t_main_data *main_data)
{
	return (!strcmp(get_cur_argv(main_data), ";"));
}

bool	cur_argv_is_pipe(t_main_data *main_data)
{
	return (!strcmp(get_cur_argv(main_data), "|"));
}

void	skip_semicolon(t_main_data *main_data)
{
	while (get_cur_argv(main_data) && cur_argv_is_semicolon(main_data))
	{
		go_to_next_argv(main_data);
	}
}

void	skip_pipe(t_main_data *main_data)
{
	while (get_cur_argv(main_data) && cur_argv_is_pipe(main_data))
	{
		go_to_next_argv(main_data);
	}
}

char	*get_cur_argv(t_main_data *main_data)
{
	return (main_data->argv[main_data->cur_argv]);
}

bool	more_argv_left(t_main_data *main_data)
{
	return (main_data->cur_argv < main_data->argc);
}

void	go_to_next_argv(t_main_data *main_data)
{
	(main_data->cur_argv)++;
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
	cur_pos = main_data->cur_argv;
	cur_argv = main_data->argv[cur_pos];
	while (cur_pos < main_data->argc && !is_semicolon(cur_argv) && !is_pipe(cur_argv))
	{
		next++;
		cur_pos++;
		cur_argv = main_data->argv[cur_pos];
	}
	return (next);
}

// void	char_arr_fill_null(char **arr, int num)
// {
// 	while (num > 0)
// 	{
// 		arr[num] = NULL;
// 		num--;
// 	}
// }
