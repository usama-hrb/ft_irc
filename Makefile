CC		= c++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98

NAME 	= ircserv

SRCS 	:= $(wildcard ./src/*.cpp ./cmds/*.cpp ./bot/*.cpp) 

OBJS	= $(SRCS:%.cpp=%.o)

OBJ_DIR = .obj/

all		: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -MMD

clean	:
	rm -f  $(SRCS:.cpp=.d)
	rm -f $(OBJS)

fclean	: clean
	rm -f $(NAME)

re		: fclean all

.PHONY	: clean

-include $(SRCS:.cpp=.d)