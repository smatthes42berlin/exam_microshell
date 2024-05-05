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
# include <unistd.h>

// data structure

typedef struct s_list	t_list;

typedef struct s_list
{
	void				*content;
	t_list				*next;
}						t_list;

typedef struct s_cmd
{
	char				**argv;
}						t_cmd;

typedef struct s_cmd_seq
{
	t_list				*cmds;
}						t_cmd_seq;

typedef struct s_main_data
{
	t_list				*cmd_seq;
	t_cmd_seq			*cur_seq;
	t_cmd				*cur_cmd;
	int					cur_argv;
	int					cur_cmd_arg;
	char				**envp;
	char				**argv;
	int					argc;
}						t_main_data;

// main

void					init_data(t_main_data *main_data, int argc,
							char *argv[], char **envp);

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

// allocate

void					allocate_cmd_seq(t_main_data *main_data);
void					allocate_cmd(t_main_data *main_data);

// list

t_list					*ft_lstnew(void *content);
void					ft_lstadd_back(t_list **lst, t_list *new);
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstdelone(t_list *lst, void (*del)(void *));
t_list					*ft_lstlast(t_list *lst);

// parser

void					parse_data(t_main_data *main_data);
void					create_sequence(t_main_data *main_data);
void					create_cmds(t_main_data *main_data);
void					add_arg_to_cur_cmd(t_main_data *main_data);

// execution

// print

void					print(t_main_data *main_data);
void					print_seq(t_cmd_seq *seq);
void					print_cmd(t_cmd *cmd);

#endif