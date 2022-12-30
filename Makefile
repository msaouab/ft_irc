# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msaouab <msaouab@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 14:08:45 by msaouab           #+#    #+#              #
#    Updated: 2022/12/30 14:44:19 by msaouab          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

bold = $(shell tput bold)
magenta = `tput setaf 5`
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
ED = \033[0m
LINE_CLEAR = \x1b[1A\x1b[M

NAME = IRC

SRC =	Testers/main.cpp

HEADERS = 

OBJ = $(SRC:.cpp=.o)

CC = c++ -std=c++98

CFLAGS = -Wall -Wextra -Werror

all : credit

credit:
	@echo "\n	╔████████████████████████████████████╗"
	@echo "	██                                   ██"
	@echo "	██       ██╗███████╗  ██████╗        ██"
	@echo "	██       ██║██╔═══██╗██╔════╝        ██"
	@echo "	██       ██║███████╔╝██║             ██"
	@echo "	██       ██║██╔══██╗ ██║             ██"
	@echo "	██       ██║██║  ╚██╗╚██████╗        ██"
	@echo "	██       ╚═╝╚═╝   ╚═╝ ╚═════╝        ██"
	@echo "	██   ┃By:$(GREEN) msaouab iqessam ygbouri$(ED)┃   ██"
	@echo "	 ╚████████████████████████████████████╝\n\n"

$(NAME): $(OBJ)
		@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo " $(bold)$(magenta)Combilation Done.$(ED) $(LINE_CLEAR)"
		@rm $(OBJ)


clean: 
		@rm -f $(OBJ) $(OBJ_It) $(OBJ_Map) $(OBJ_RBT) $(OBJ_Vector) $(OBJ_Set) $(OBJ_Stack)
		@echo " $(bold)$(magenta)clean Done.$(ED) $(LINE_CLEAR)"

fclean: clean
		@rm -f $(NAME) $(NAME_V) $(NAME_S) $(NAME_M) $(NAME_R) $(NAME_Se) $(NAME_I)
		@echo " $(bold)$(magenta)fclean Done.$(ED) $(LINE_CLEAR)"

re: fclean all
