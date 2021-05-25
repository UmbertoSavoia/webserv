
#		Makefile Webserv

NAME		=	webserv
CXX			=	clang++
CXXFLAGS	=	-std=c++11 -g -fsanitize=address #-Wall -Wextra -Werror

MKDIR		=	mkdir -p
RM			=	rm -f
RMDIR		=	rm -rf

SRC_DIR		=	src
OBJ_DIR		=	obj

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

SRC			=	$(foreach dir, $(SRC_DIR), $(foreach file, $(wildcard $(dir)/*.cpp), $(notdir $(file))))
OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))
HPP			=	$(wildcard include/*.hpp)

all: $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	@$(MKDIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(HPP)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
