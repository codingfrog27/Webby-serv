# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/10 18:17:45 by mde-cloe          #+#    #+#              #
#    Updated: 2024/09/06 17:47:09 by mde-cloe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

CC = c++
Wflags = -Wall -Wextra -Werror -g -fsanitize=address
VERSION_FLAG = -std=c++20


INCLUDE = -I include/ -I libft/include
LIBFT_A			:=	./libft/libft.a


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

$(NAME): $(SOURCES) $(OFILES) $(LIBFT_A)
	@$(CC) $(Wflags) $(INCLUDE) $(VERSION_FLAG) $(OFILES) -o $(NAME)
	@printf "$(C_BLUE)$(NAME) $(C_GREEN) Compiled!\n\n$(C_RESET )"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	@printf "$(C_GREEN) Compiling $(C_BLUE)$<\n$(C_RESET)"
	@$(CC) -c $< $(Wflags) $(INCLUDE) $(VERSION_FLAG) -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBFT_A):
	@$(MAKE) -C libft

re: fclean $(NAME)

run: $(NAME)
	@./$(NAME)

clean:
	@rm -rf $(OBJDIR)
	@printf "$(C_RED)$(OBJDIR) removed \n\n$(C_RESET )"
fclean: clean
	@rm -rf $(NAME)
	@printf "$(C_RED)$(NAME) removed \n\n$(C_RESET )"
