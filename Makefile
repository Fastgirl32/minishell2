NAME = minishell
LIBNAME = libft/libft.a

CFLAGGEN = -Wall -Wextra -Werror -g

CC = cc

QUELLE_DATEIEN = builtins/ft_cd.c builtins/ft_env.c builtins/ft_export.c builtins/ft_unset.c builtins/ft_exit.c builtins/ft_pwd.c builtins/ft_echo.c \
utils.c banner.c \
find_and_exec.c \
minishell.c \
init.c

OBJEKT_DATEIEN = $(QUELLE_DATEIEN:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGGEN) -c $< -o $@

libft/libft.a:
	cd libft && make

$(NAME): $(OBJEKT_DATEIEN) libft/libft.a
	$(CC) $(CFLAGGEN) $(OBJEKT_DATEIEN) $(LIBNAME) -lreadline -o $(NAME)

clean: 
	rm -f $(OBJEKT_DATEIEN)
	cd libft && make clean

fclean: clean 
	rm -f $(NAME)
	rm -f $(LIBNAME)

re: fclean all

.PHONY: all clean flcean re
