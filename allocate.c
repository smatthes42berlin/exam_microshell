#include "header.h"

void	allocate_cmd_seq(t_main_data *main_data)
{
	t_cmd_seq	*new_seq;
	t_list		*new_lst;

	new_seq = malloc(sizeof(t_cmd_seq));
	if (!new_seq)
		free_exit(1, main_data);
	new_lst = ft_lstnew(new_seq);
	if (!new_lst)
		free_exit(1, main_data);
	ft_lstadd_back(&(main_data->cmd_seq), new_lst);
	new_seq->cmds = NULL;
	main_data->cur_seq = new_seq;
}

void	allocate_cmd(t_main_data *main_data)
{
	t_list *new_lst;

	int next_pipe_or_semicolon = find_next_pipe_or_semi_or_end(main_data);
	t_cmd *new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		free_exit(1, main_data);
	new_cmd->argv = malloc(sizeof(char **) * (next_pipe_or_semicolon + 1));
	if (!new_cmd->argv)
		free_exit(1, main_data);
	new_cmd->argv[next_pipe_or_semicolon] = NULL;
	new_lst = ft_lstnew(new_cmd);
	if (!new_lst)
		free_exit(1, main_data);
	ft_lstadd_back(&(main_data->cur_seq->cmds), new_lst);
	main_data->cur_cmd = new_cmd;
	main_data->cur_cmd_arg = 0;
}