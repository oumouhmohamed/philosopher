CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
LDFLAGS = -lpthread
SRCS = philosopher.c utils.c utils1.c help_thread.c \
		help_thread1.c
OBJ = $(SRCS:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(objs)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o:%.c philosopher.h
	$(CC) $(CFLAGS) -c  $<  -o $@

$(NAME) : $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
