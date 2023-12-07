NAME = ircserv

SRCS = srcs/main.cpp srcs/IrcServ.cpp srcs/IrcUser.cpp srcs/Token.cpp srcs/IrcChannel.cpp srcs/display_cmds.cpp

OBJ_DIR	= objs/

SRCS_DIR = srcs/

OBJS1 = $(subst srcs/,objs/,$(SRCS))

OBJS = $(subst .cpp,.o,$(OBJS1))

DEPS = $(shell find . -maxdepth 2 -type f -regex ".*\.d")

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./includes

RM = /bin/rm -rf

.PHONY: all clean fclean re srcs

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

-include $(DEPS)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp
	@if [ ! -d "$(OBJ_DIR)" ]; then \
	echo "objects folder not found."; \
	echo "creating objects folder ..."; \
	mkdir $(OBJ_DIR); \
	fi;
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	echo "deleting objects folder ..."; \
	$(RM) $(OBJ_DIR); \
	fi;

fclean: clean
	$(RM) $(NAME)

re: fclean all
