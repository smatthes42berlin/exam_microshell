#include "header.h"

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