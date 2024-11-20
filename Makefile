NAME := webserv

CC = c++
Wflags = -Wall -Wextra
VERSION_FLAG = -std=c++20


INCLUDE = -I include/ -I libft/include
LIBFT_A			:=	./libft/libft.a
Wflags := #-g -fsanitize=address
Wflags := -Wall -Wextra -Werror -g -fsanitize=address


SRCDIR = sources
OBJDIR = objects
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# ifneq (,$(filter debug strict,$(MAKECMDGOALS)))
# endif


#-----colours----

C_GREEN = \033[0;32m
C_RED = \033[0;31m
C_BLUE = \033[0;34m
C_RESET = \033[0m

#-----------------

$(NAME): $(SOURCES) $(OBJECTS) $(LIBFT_A)
	@$(CC) $(Wflags) $(INCLUDE) $(VERSION_FLAG) $(OBJECTS) -o $(NAME)
	@printf "$(C_BLUE)$(NAME) $(C_GREEN) Compiled!\n\n$(C_RESET )"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
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