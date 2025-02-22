CC		= c++

CPPFLAGS	= #-Wall -Wextra -Werror -std=c++98

NAME 	= ircserv

SRCS	= src/main.cpp src/Server.cpp 

OBJS	= $(SRCS:%.cpp=%.o)

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