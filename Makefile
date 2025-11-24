NAME = webserv

SRCS = main.cpp \
		http/Http.cpp \
		coreEngine/CoreEngine.cpp \
		coreEngine/poll.cpp \
		http/stringUtils.cpp \
		configReader/config.cpp \
		configReader/printConfig.cpp \
		utils/utils.cpp


OBJS = ${SRCS:.cpp=.o}

CC = g++
CFLAGS = -g -Wall -Wextra -Werror -std=c++98
RM = rm -f

all: $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
