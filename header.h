#ifndef HEADER_H
# define HEADER_H

// allowed Funcs
// malloc,
// free,
// write,
// close,
// fork,
// waitpid,
// signal,
// kill,
// exit,
// chdir,
// execve,
// dup,
// dup2,
// pipe,
// strcmp,
// strncmp

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// data structure

typedef struct s_list	t_list;

# define PIPE_LIMIT 1000
# define PROC_LIMIT 1000
# define ARGV_LIMIT 1000
# define DEFAULT_INT -2

typedef struct s_list
{
	void				*content;
	t_list				*next;
}						t_list;

typedef struct s_cmd
{
	char				*name;
	char				*argv[ARGV_LIMIT];
}						t_cmd;

typedef struct s_main_data
{
	int					num_cur_argv;
	char				*cur_argv;
	int					proc[PROC_LIMIT];
	int					pipes[PIPE_LIMIT][2];
	int					num_proc;
	int					pipe_in[2];
	int					pipe_out[2];
	bool				more_cmd_left;
	char				**envp;
	char				**argv;
	int					argc;
}						t_main_data;

/* init */

void					init_data(t_main_data *main_data, int argc, char **argv,
							char **envp);

/* execute main */

int						execute_main(t_main_data *main_data);
int						create_cmd(t_main_data *main_data);
int						execute_cmd(t_main_data *main_data, t_cmd cmd);
int						child_execute(t_main_data *main_data, t_cmd *cmd);
int						redirect_in(t_main_data *main_data);
int						redirect_out(t_main_data *main_data);

// util

void					skip_semicolon(t_main_data *main_data);
void					skip_pipe(t_main_data *main_data);
bool					is_semicolon(char *str);
bool					is_pipe(char *str);
void					free_exit(int code, t_main_data *main_data);
bool					cur_argv_is_semicolon(t_main_data *main_data);
bool					cur_argv_is_pipe(t_main_data *main_data);
char					*get_cur_argv(t_main_data *main_data);
void					go_to_next_argv(t_main_data *main_data);
bool					more_argv_left(t_main_data *main_data);
int						find_next_pipe_or_semi_or_end(t_main_data *main_data);

// list

t_list					*ft_lstnew(void *content);
void					ft_lstadd_back(t_list **lst, t_list *new);
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstdelone(t_list *lst, void (*del)(void *));
t_list					*ft_lstlast(t_list *lst);

#endif