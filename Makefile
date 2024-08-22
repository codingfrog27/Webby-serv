# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: mde-cloe <mde-cloe@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/04/10 18:17:45 by mde-cloe      #+#    #+#                  #
#    Updated: 2023/04/10 18:17:45 by mde-cloe      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME := webserv

CC = c++

Wflags = -Wall -Wextra -Werror -g -fsanitize=address

INCLUDE = -I include/

VERSION_FLAG = -std=c++20

SRCDIR = sources
OBJDIR = objects


SOURCES = $(wildcard $(SRCDIR)/*.cpp) #tmp cuz idk about subfolders and wildcard
OFILES	:=	$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))


#-----colours----

C_GREEN = \033[0;32m
C_RED = \033[0;31m
C_BLUE = \033[0;34m
C_RESET = \033[0m

#-----------------

$(NAME): $(SOURCES) $(OFILES)
	@$(CC) $(Wflags) $(INCLUDE) $(VERSION_FLAG) $(OFILES) -o $(NAME)
	@printf "$(C_BLUE)$(NAME) $(C_GREEN) Compiled!\n\n$(C_RESET )"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	@printf "$(C_GREEN) Compiling $(C_BLUE)$<\n$(C_RESET)"
	@$(CC) -c $< $(Wflags) $(INCLUDE) $(VERSION_FLAG) -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

re: fclean $(NAME)

run: $(NAME)
	@./$(NAME)

clean:
	@rm -rf $(OBJDIR)
	@printf "$(C_RED)$(OBJDIR) removed \n\n$(C_RESET )"
fclean: clean
	@rm -rf $(NAME)
	@printf "$(C_RED)$(NAME) removed \n\n$(C_RESET )"
