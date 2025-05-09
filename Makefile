NAME := webserv

CC = c++
Wflags = -Wall -Wextra
VERSION_FLAG = -std=c++20

INCLUDE_FLAGS = -I include/ -I libft/include
LIBFT_A			:=	./libft/libft.a
Wflags = -Wall -Wextra -Werror
Wflags += -g -fsanitize=address

INCLDIR := include/
SRCDIR = sources
OBJDIR = objects
HEADERS := $(shell find $(INCLDIR) -name '*.hpp')
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
	@$(CC) $(Wflags) $(INCLUDE_FLAGS) $(VERSION_FLAG) $(OBJECTS) -o $(NAME)
	@printf "$(C_BLUE)$(NAME) $(C_GREEN) Compiled!\n\n$(C_RESET )"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@mkdir -p $(dir $@)
	@printf "$(C_GREEN) Compiling $(C_BLUE)$<\n$(C_RESET)"
	@$(CC) -c $< $(Wflags) $(INCLUDE_FLAGS) $(VERSION_FLAG) -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBFT_A):
	@$(MAKE) -C libft

re: fclean $(NAME)

run: $(NAME)
	@./$(NAME) example_config.conf

clean:
	@rm -rf $(OBJDIR)
	@printf "$(C_RED)$(OBJDIR) removed \n\n$(C_RESET )"
fclean: clean
	@rm -rf $(NAME)
	@printf "$(C_RED)$(NAME) removed \n\n$(C_RESET )"