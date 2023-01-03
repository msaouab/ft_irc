
NAME = ircserv

BOLD = $(shell tput bold)
MAG = `tput setaf 5`
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
ED = \033[0m
LINE_CLEAR = \x1b[1A\x1b[M

CC = c++ -std=c++98
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRC =	./src/main.cpp\
		./src/server.cpp\
		./src/utils.cpp

HEADERS = Includes/ircserv.hpp

OBJ = $(SRC:.cpp=.o)

all : credit $(NAME)

credit:
	@echo "\n$(BLUE)\t███████████████████████████████████████"
	@echo "$(BLUE)\t█ $(YELLOW)╔════╗╔══╦══╗   ╔══╦══╗╔══╗  ╔════╗$(ED) $(BLUE)█$(ED)"
	@echo "$(BLUE)\t█ $(YELLOW)║        ║         ║   ║  ║  ║     $(ED) $(BLUE)█$(ED)"
	@echo "$(BLUE)\t█ $(YELLOW)╠════    ║         ║   ╠══╩═╗║     $(ED) $(BLUE)█$(ED)"
	@echo "$(BLUE)\t█ $(YELLOW)║        ║ ════ ╚══╩══╝║    ║╚════╝$(ED) $(BLUE)█$(ED)"
	@echo "$(BLUE)\t█  ┃By:$(GREEN) msaouab - iqessam - ygbouri$(ED)$(BLUE)┃  █$(ED)"
	@echo "$(BLUE)\t███████████████████████████████████████\n\n$(ED)"

$(NAME): $(OBJ) $(HEADERS)
		@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "$(LINE_CLEAR) $(BOLD)$(MAG)Compilation Done.$(ED)"

clean: 
		@$(RM) $(OBJ)
		@echo " $(BOLD)$(MAG)clean Done.$(ED) $(LINE_CLEAR)"

fclean: clean
		@$(RM) $(NAME)
		@echo "$(LINE_CLEAR)$(BOLD)$(MAG)fclean Done.$(ED) $(LINE_CLEAR)"

re: fclean all

.PHONY: all clean fclean re
