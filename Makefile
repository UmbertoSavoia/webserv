
#		Makefile Webserv

NAME		=	webserv
CXX			=	clang++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++11 #-g

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
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "Compiling... %-33.33s\r\n"

$(NAME): $(OBJ) $(HPP)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[0;32mCreating		Webserv\033[0;0m"

clean:
	@$(RMDIR) $(OBJ_DIR)
	@echo "\033[0;31mCleaning		obj folder\033[0;0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[0;31mRemoving		Webserv\033[0;0m"

re: fclean all

.PHONY: all clean fclean re
