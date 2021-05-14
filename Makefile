
#		Makefile Webserv

NAME		=	webserv
CXX			=	clang++
CXXFLAGS	=	-std=c++11 -g#-Wall -Wextra -Werror
RM			=	rm -f

SRC			=	$(wildcard src/*.cpp)
HPP			=	$(wildcard include/*.hpp)
OBJ			=	$(SRC:.cpp=.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJ) $(HPP)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re