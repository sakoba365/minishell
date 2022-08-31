CC = GCC

CFLAGS = -Wall -Wextra -Werror

INCLUDES = minishell.h

NAME = minishell.a

SOURCES = minishell.c \
		  ./built-in/shell_echo.c \
		  ./built-in/shell_exit.c \
		  ./built-in/shell_pwd.c \
		  ./built-in/shell_env.c \
		  ./built-in/shell_export.c \
		  ./built-in/shell_unset.c \
		  ./built-in/shell_cd.c \
		  ./parser/parser.c \
		  ./exec/runner.c \
		  ./parser/parser-util.c \
		  ./redirect/redir.c \
		  ./filter/filter.c

OBJECTS = $(SOURCES:.c=.o)

LIBFT_PATH = ./libft/

all: subsystem $(NAME)


subsystem :
	make -C ./libft
	cp ./libft/libft.a libft.a

$(NAME): subsystem $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) libft.a -L/usr/local/lib -I/usr/local/include -lreadline -framework AppKit -o minishell


clean:
	rm -f $(OBJECTS)
	rm -f $(LIBFT_PATH)*.o

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT_PATH)libft.a
	rm -f libft.a
	rm -f minishell

re: fclean all
