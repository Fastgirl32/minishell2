/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:51:14 by lstarek           #+#    #+#             */
/*   Updated: 2026/07/24 12:40:40 by saecker          ###   ########.fr       */
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
# include <sys/wait.h>
# include <unistd.h>
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
	int					history_count;
	int					history_cap;
	int					stop;
	t_command			*list;
}						t_vars;

struct					s_expand
{
	const char			*line;
	size_t				i;
	size_t				end;
	char				**env;
	char				*dyn;
	size_t				len;
	size_t				cap;
	char				quote;
};

struct					s_split
{
	const char			*line;
	size_t				end;
	char				**env;
	int					*ac;
	char				**av;
	size_t				i;
	size_t				j;
};

struct					s_redir
{
	t_command			**head;
	t_command			**tail;
	t_vars				*vars;
	char				**av;
	int					ac;
	int					has_pipe;
	int					op_i;
};

int						ft_strcmp(const char *s1, const char *s2);

char					*key(const char *str);
char					*value(const char *str);
char					*get_var(char *var, char **env);

t_u16					is_builtin(char *cmd);

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
void					execute_builtin(t_command *cmd, char **env);
void					execute(t_command *cmd, char **env_src);

char					*get_input(int fd);

void					print_banner(void);

void					setup_parent_signals(void);
void					setup_child_signals(void);

char					**recreate_env(char **env);
void					env_append(char ***env, char *str);
void					env_remove(char ***env, char *str);
void					free_arr(void **arr);
void					print_env_fd(int fd, char *format, char **env);

void					input_process(t_vars *vars);
void					make_list(t_vars *vars, char *line);
t_vars					*init_vars(char **env);

int						is_blank(char c);
int						is_blank_line(const char *s);
int						is_var_char(char c);
int						redir_op_len(const char *line, size_t i, char quote);
size_t					segment_end(const char *line, size_t start);
size_t					next_word_end(const char *line, size_t i, size_t end);
size_t					count_tokens(const char *line, size_t start,
							size_t end);

int						append_char(char **str, size_t *len, size_t *cap,
							char c);
int						append_str(char **str, size_t *len, size_t *cap,
							const char *src);
char					*check_env(char **env, const char *name);
int						append_status(char **dyn, size_t *len, size_t *cap);
int						append_env_var(struct s_expand *ex);
int						handle_dollar(struct s_expand *ex);
int						consume_quote_char(struct s_expand *ex);
int						append_token_piece(struct s_expand *ex);
char					*alloc_token_buffer(void);
void					setup_expand(struct s_expand *ex, const char *line,
							char **env, char *dyn);
char					*copy_token(const char *line, size_t start, size_t end,
							char **env);

char					*read_line_prompt(t_vars *vars, const char *prompt);
char					*read_line_plain(void);
char					*read_shell_line(t_vars *vars, const char *prompt);
char					*append_line(char *line, char *more);
const char				*quote_prompt(char q);
char					find_unclosed_quote(const char *s);
char					*read_continued_lines(t_vars *vars, char *line);

int						delimiter_found(const char *line,
							const char *delimiter);
char					*read_heredoc_line(void);
int						write_heredoc_line(int fd, const char *line);
void					replace_cmd_fd_in(t_command *cmd, int new_fd);
int						fill_heredoc_pipe(int fd, const char *limiter);
int						setup_heredoc(t_command *cmd);
int						prepare_heredocs(t_command *head);
void					free_lines_partial(char **lines, int count);
char					**grow_lines(char **lines, int count, char *line);
char					*normalize_heredoc_line(char *line);
int						collect_heredoc_line(char ***lines, int *count,
							const char *limiter);
char					**collect_heredoc_lines(const char *limiter,
							int *out_count);
t_command				*alloc_heredoc_cmd(char **lines, int count);
t_command				*build_heredoc(const char *limiter);

char					*trimmed_history_entry(const char *line);
int						history_reserve(t_vars *vars);
void					history_add(t_vars *vars, const char *line);
void					history_print(t_vars *vars);

void					free_list(t_command *cmd);
int						set_pipe_limiter(t_command *cmd, int has_pipe);
int						fill_command_base(t_command *cmd, char **av, int *ac,
							int has_pipe);
char					**dup_token_range(char **av, int start, int end,
							int *out_ac);
t_command				*new_command(char **av, int ac, int has_pipe);
int						first_redir_index(char **av, int ac);
int						set_command_limiter(t_command *cmd, char *lim);
t_command				*new_single_arg_command(char *arg);
int						append_redirect_nodes(struct s_redir *rd);
int						handle_redirect_segment(struct s_redir *rd);
int						has_pipe_after_segment(const char *line, size_t end);
void					append_plain_segment(struct s_redir *rd);
int					consume_redir_only_segment(struct s_redir *rd);


void					parse_segment(struct s_redir *rd, const char *line,
							size_t start);
size_t					skip_blanks(const char *line, size_t i);
size_t					parse_and_move(struct s_redir *rd, const char *line,
							size_t i);
size_t					next_segment_start(const char *line, size_t pos);
t_command				*build_command_list(t_vars *vars, const char *line);
void					execute_single_command(t_command *head, t_vars *vars);
int						should_skip_list(t_command *head, t_vars *vars);

char					*safe_text(char *s);
void					print_one_command(int idx, t_command *head);
void					append_command(t_command **head, t_command **tail,
							t_command *new_cmd);
void					print_argv_debug(char **argv);
void					print_command_list(t_command *head);

int						is_redirect_op(const char *s);
int						is_heredoc_op(const char *s);
void					set_redirect_limit(char **limit, char **av, int i,
							int ac);
int						skip_redirect_token(char **av, int i, int ac,
							char **limit);
void					extract_redirections(char **av, int *ac, char **limit,
							int *redirect_start);

int						push_operator_token(struct s_split *sp);
int						push_word_token(struct s_split *sp);
int						fill_split_tokens(struct s_split *sp);
char					**split_tokens(const char *line, size_t start,
							size_t end, struct s_split *sp);
void					free_tokens(char **av, size_t used);

extern int				g_status;

#endif // MINISHELL_H