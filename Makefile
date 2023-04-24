NAME	= pipex

SRCS	= main.c utils.c
OBJS	= ${SRCS:.c=.o}
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror
RM		= rm -rf

all: ${NAME}

${NAME}: ${OBJS}
	@$(MAKE) -sC ./libft
	@${CC} ${OBJS} ${CFLAGS} ./libft/libft.a -o ${NAME}

clean:
	@$(MAKE) -C libft fclean
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all

bonus: all

.PHONY: clean fclean re all bonus norme