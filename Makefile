
SRC =		main.c	\
			src/parsing.c src/codexion.c src/instantiations.c		\
			src/monitor.c src/coders.c src/utils.c src/display.c	\
			src/dongles.c

OBJ =		$(SRC:.c=.o)

CFLAGS =	-Wall -Wextra -Werror -pthread -g
NAME =		codexion.out

all: $(NAME)
	./codexion.out 5 800 100 100 100 4 50 edf

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
	rm -f vgcore.*

fclean: clean
	rm -f $(NAME) vgcore.* 

re: fclean all

.PHONY: all clean fclean re
