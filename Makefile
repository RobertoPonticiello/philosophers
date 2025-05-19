CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $^

clean:
    rm -f $(OBJS)

fclean: clean
    rm -f $(NAME)

re: fclean all
