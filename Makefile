
SRC =		main.c	\
			src/parsing.c

OBJ =		$(SRC:.c=.o)

CFLAGS =	-Wall -Wextra -Werror -pthread
NAME =		codexion.out

all: $(NAME)

no-flag:
	cc $(SRC) -o $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
	rm -f vgcore.*

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all no-flag clean fclean re
