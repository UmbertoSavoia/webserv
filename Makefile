
#		Makefile Webserv

NAME		=	webserv
CXX			=	clang++
CXXFLAGS	=	-std=c++11 #-Wall -Wextra -Werror
RM			=	rm -f

SRC			=	$(wildcard src/*.cpp)
OBJ			=	$(SRC:.cpp=.o)

%.o : %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re