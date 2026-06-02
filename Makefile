NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I include

SRC = src/main.c \
	src/parse.c \
	src/time.c \
	src/heap.c \
	src/init.c \
	src/cleanup.c \
	src/log.c \
	src/dongle.c \
	src/claim.c \
	src/acquire.c \
	src/routine.c \
	src/monitor.c \
	src/thread.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c include/codexion.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
