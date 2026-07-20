/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baal <baal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:51:14 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/09 21:22:33 by baal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
#include <sys/wait.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef unsigned short	t_u16;
typedef unsigned char	t_status;

// command = erwartet builtin wie "echo" oder name von executabe "printf"
// ac = ac. 1 weniger als "echte länge" vom array, weil name & nullterminator.
// argv = konvention: erstes argument = command, letztes = NULL.
// next = pointer zu nächstem kommand
// fd in & out: nicht beachten.
typedef struct s_command
{
	char				*command;
	char				*limiter;
	int					ac;
	char				**argv;
	struct s_command	*next;
	int					fd_in;
	int					fd_out;
}						t_command;

typedef struct s_vars
{
	char				**env;
	char				**history;
	int					 history_count;
	int					 history_cap;
	int					stop;
	t_command			*list;
}						t_vars;

int						ft_strcmp(const char *s1, const char *s2);

char					*key(const char *str);
char					*value(const char *str);
char					*get_var(char *var, char **env);

t_u16	is_builtin(char *cmd);

t_status				ft_echo(t_command *cmd, char **env);
t_status				ft_cd(t_command *cmd, char **env);
t_status				ft_pwd(t_command *cmd);
t_status				ft_export(t_command *cmd, char ***env);
t_status				ft_unset(t_command *cmd, char ***env);
t_status				ft_env(t_command *cmd, char **env);
t_status				ft_exit(t_command *cmd);

t_status				find_and_exec(t_command *cmd, char **env);

t_u16					connect_pipes(t_command *top_cmd);
t_u16					establish_redirects(t_command *top_cmd);
void	execute_builtin(t_command *cmd, char **env);
void					execute(t_command *cmd, char **env_src);

char					*get_input(int fd);

void					print_banner(void);

void				setup_parent_signals(void);
void				setup_child_signals(void);

char					**recreate_env(char **env);
void					env_append(char ***env, char *str);
void					env_remove(char ***env, char *str);
void					free_arr(void **arr);
void					print_env_fd(int fd, char *format, char **env);

void					get_line(t_vars *vars);
t_vars					*init_vars(char **env);

extern int				g_status;

#endif // MINISHELL_H