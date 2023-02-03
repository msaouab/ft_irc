
NAME = ircserv

BOLD = $(shell tput bold)
MAG = `tput setaf 5`
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
ED = \033[0m
LINE_CLEAR = \x1b[1A\x1b[M

CC = c++ 

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

SRC =	./src/main.cpp\
		./src/server.cpp\
		./src/Client.cpp\
		./src/Bot.cpp\
		./src/utils.cpp\
		./src/Channel.cpp\
		./src/ft_split.cpp\
		./src/handl_message.cpp\
		./src/Commands.cpp
		# ./src/dcc.cpp\

HEADERS = Includes/ircserv.hpp

OBJ = $(SRC:.cpp=.o)

all : credit $(NAME)

credit:
	@echo "\n"
	@echo "$(GREEN)\r\t╔════╗╔══╦══╗   ╔══╦══╗╔══╗  ╔════╗$(ED)"
	@echo "$(GREEN)\r\t║        ║         ║   ║  ║  ║     $(ED)"
	@echo "$(GREEN)\r\t╠════    ║         ║   ╠══╩═╗║     $(ED)"
	@echo "$(GREEN)\r\t║        ║ ════ ╚══╩══╝║    ║╚════╝$(ED)"
	@echo "$(GREEN)\r\t┃ $(ED)By: $(MAG)msaouab - iqessam - ygbouri$(GREEN) ┃$(ED)\n"

$(NAME): $(OBJ) $(HEADERS)
		@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "$(LINE_CLEAR) $(BOLD)$(MAG)\r\t Compilation Done.$(ED)"

%.o:%.cpp $(HEADERS)
	@printf "$(YELLOW)Compiling$(ED) $<:\r\t\t\t\t\t..."
	@$(CC) $(CFLAGS) -o $@ -c $<
	@echo "\r\t\t\t\t\t$(GREEN){DONE}$(ED)"

clean: 
		@$(RM) $(OBJ)
		@echo " $(BOLD)$(MAG)clean Done.$(ED) $(LINE_CLEAR)"

fclean: clean
		@$(RM) $(NAME)
		@echo "$(LINE_CLEAR)$(BOLD)$(MAG)fclean Done.$(ED) $(LINE_CLEAR)"

re: fclean all

.PHONY: all clean fclean re
