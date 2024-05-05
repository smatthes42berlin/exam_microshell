#include "header.h"

void	print(t_main_data *main_data)
{
	t_list	*cur_elem;

	printf("\n\nMain Data\n");
	if (!(main_data->cmd_seq))
		printf("No cmd squences\n");
	else
	{
		cur_elem = main_data->cmd_seq;
		while (cur_elem)
		{
			printf("1\n");
			print_seq(cur_elem->content);
			cur_elem = cur_elem->next;
		}
	}
}

void	print_seq(t_cmd_seq *seq)
{
	t_list	*cur_elem;

	printf("Seq\n");
	printf("cmds %p\n\n", seq->cmds);
	cur_elem = seq->cmds;
	while (cur_elem)
	{
		print_cmd(cur_elem->content);
		cur_elem = cur_elem->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("Cmd\n\n");
	while (cmd->argv && cmd->argv[i])
	{
		printf("arg %d %s\n", i, cmd->argv[i]);
		i++;
	}
}
