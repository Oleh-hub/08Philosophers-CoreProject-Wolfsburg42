# threads, mutex, philosophers refs:
# https://suspectedoceano.notion.site/Philosophers-b1bf3c57eee6420cafa7d0900b3d3216
# in Russian
# https://www.youtube.com/watch?v=WzsPbwIwtiw 

NAME	:=	philo
CC		:=	cc
SRCS	:=	philo.c			\
			philo_utils.c	

OBJS	:=	$(SRCS:.c=.o)
OBJS_DIR:=	obj/
OBJS_PR	:=	$(addprefix $(OBJS_DIR), $(OBJS))
# CFLAGS	:=	-Wall -Wextra -Werror -g
# CFLAGS	:=	-Wall -Wextra -Werror -g -pthread
CFLAGS	:=	-Wall -Wextra -Werror -g  -fsanitize=thread  ###
# CFLAGS	:=	-Wall -Wextra -Werror -g  -fsanitize=address
# lldb ./philo 4 410 200 200 200  # debugging from terminal

all : $(NAME)

$(NAME) : $(OBJS_PR) 
# $(CC) $(OBJS_PR) -O3  -o $(NAME)
# $(CC) $(OBJS_PR) -O3  -pthread -o $(NAME)
	$(CC) $(CFLAGS) $(OBJS_PR) -O3 -o $(NAME)

$(OBJS_DIR)%.o : %.c philo.h
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -O3 -c $< -o $@

clean :
	rm -rf $(OBJS_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

norm :
#	norminette $(SRCS) philo.h
	norminette philo.c philo.h
