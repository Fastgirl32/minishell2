NAME = minishell
LIBNAME = libft/libft.a

CFLAGGEN = -Wall -Wextra -Werror -g
funny: CFLAGGEN += -D DISPLAY_BANNER=1
verbose: CFLAGGEN += -D VERBOSE=1

CC = cc

SUPRESSION_FILE = readline.supp

QUELLE_DATEIEN = builtins/ft_cd.c builtins/ft_env.c builtins/ft_export.c builtins/ft_unset.c builtins/ft_exit.c builtins/ft_pwd.c builtins/ft_echo.c \
builtins/ft_atol.c \
execute/execute_utils.c execute/banner.c execute/environment.c execute/expand.c \
execute/find_and_exec.c execute/key_value.c execute/free.c execute/connect.c execute/connect_helpers.c \
execute/heredoc.c \
minishell.c \
input/shared.c input/signals.c input/history_core.c \
input/expand_base.c input/expand_token.c input/expand_copy.c \
input/token_scan.c input/token_split.c \
input/input_read.c input/input_process.c \
	input/debug.c input/command_build.c input/build_list.c \
	input/parser_flow.c input/parser_helpers.c input/syntax_check.c \
input/heredoc_args.c \
input/heredoc_file.c \
	input/redirect_build.c \
	input/redirect_only.c \
	input/signals_child.c execute/exec.c \
	input/signals_heredoc.c input/heredoc_helper.c input/find_and_close_quotes.c

OBJEKT_DATEIEN = $(QUELLE_DATEIEN:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGGEN) -c $< -o $@

libft/libft.a:
	cd libft && make

$(NAME): $(OBJEKT_DATEIEN) libft/libft.a
	$(CC) $(CFLAGGEN) $(OBJEKT_DATEIEN) $(LIBNAME) -lreadline -o $(NAME)

funny: $(NAME)
verbose: $(NAME)

clean: 
	rm -f $(OBJEKT_DATEIEN)
	rm -f $(SUPRESSION_FILE)
	cd libft && make clean

fclean: clean 
	rm -f $(NAME)
	rm -f $(LIBNAME)

re: fclean all

vg: | $(SUPRESSION_FILE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=all --suppressions=readline.supp ./minishell

$(SUPRESSION_FILE):
	printf "{\n\t<readline-faxen>\n\tMemcheck:Leak\n\t...\n\tfun:readline\n}\n{\n\t<readline-internal-faxen>\n\tMemcheck:Leak\n\t...\n\tfun:readline_internal_char\n}\n{\n\t<history-faxen>\n\tMemcheck:Leak\n\t...\n\tfun:add_history\n}" | cat > readline.supp

.PHONY: all clean flcean re
