#include "header.h"

void	parse_data(t_main_data *main_data)
{
	while (more_argv_left(main_data))
	{
		skip_semicolon(main_data);
		if (more_argv_left(main_data))
			create_sequence(main_data);
	}
	printf("end parser\n");
}

void	create_sequence(t_main_data *main_data)
{
	allocate_cmd_seq(main_data);
	while (more_argv_left(main_data) && !cur_argv_is_semicolon(main_data))
	{
		skip_pipe(main_data);
		create_cmds(main_data);
	}
}

void	create_cmds(t_main_data *main_data)
{
	allocate_cmd(main_data);
	while (more_argv_left(main_data) && !cur_argv_is_semicolon(main_data)
		&& !cur_argv_is_pipe(main_data))
	{
		add_arg_to_cur_cmd(main_data);
		go_to_next_argv(main_data);
	}
}

void	add_arg_to_cur_cmd(t_main_data *main_data)
{
	printf("adding arg: %s\n", get_cur_argv(main_data));
	main_data->cur_cmd->argv[main_data->cur_cmd_arg] = get_cur_argv(main_data);
	main_data->cur_cmd_arg++;
}
