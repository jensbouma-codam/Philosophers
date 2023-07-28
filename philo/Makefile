# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbouma <jbouma@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/10/10 14:09:40 by jbouma        #+#    #+#                  #
#    Updated: 2023/07/28 11:32:56 by jensbouma     ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# Program Name(s)
NAME		:= philo

# Libraries
LIBDIR		:= lib
LIBS		:= libvector

# Sources
FILES		= 	input.c			\
				main.c			\
				msg.c			\
				philo.c			\
				forks.c			\
				sim.c			\
				tools/error.c	\
				tools/mem.c		\
				tools/time.c	\
				tools/value.c	\
				tools/string.c

# Set directories
BUILDDIR	:= build
SRCDIR		:= src

# Compiler Settings
CC 			:= gcc
CFLAGS		+= -Werror -Wall -Wextra
CFLAGS		+= -O3

# Dynamic files
SOURCES		= ${addprefix $(SRCDIR)/, $(FILES)}
LIBS_A		= $(addsuffix /*.a, $(addprefix $(LIBDIR)/, $(LIBS)))
LIBS_INC	= $(addsuffix /include, $(addprefix $(LIBDIR)/, $(LIBS)))

# Headers
INC 		= -I include -I$(LIBS_INC)

# Objects
OBJECTS		= $(SOURCES:%.c=$(BUILDDIR)/%.o)

# Colors
ifneq (,$(findstring xterm,${TERM}))
	BLACK		:= $(shell tput -Txterm setaf 0)
	RED			:= $(shell tput -Txterm setaf 1)
	GREEN		:= $(shell tput -Txterm setaf 2)
	YELLOW		:= $(shell tput -Txterm setaf 3)
	LIGHTPURPLE	:= $(shell tput -Txterm setaf 4)
	PURPLE		:= $(shell tput -Txterm setaf 5)
	BLUE		:= $(shell tput -Txterm setaf 6)
	WHITE		:= $(shell tput -Txterm setaf 7)
	RESET		:= $(shell tput -Txterm sgr0)
endif

P				:= printf "%-25.25s%s\n"
P_OK			= $(P) "$@${GREEN}" "Norm OK" "${RESET}"
P_KO			= $(P) "$@${RED}" "Norm KO" "${RESET}"

# Rules
all: $(NAME)
	@printf "\n🙏 $(PURPLE)All Done $(RESET)\n"
	

$(NAME): $(OBJECTS)
	@make $(LIBS)
	@$(CC) $(CFLAGS) $(INC) $(HEADERS) $(OBJECTS) $(LIBS_A) $(GLFW) -o $(NAME)	\
		&& ($(P) "Executable $(GREEN)" "$@ $(RESET)" && exit 0)			\
		|| ($(P) "Executable $(RED)" "$@ Compile error $(RESET)" && exit 1)
	@$(P) "Flags $(YELLOW)" "$(CFLAGS) $(RESET)"

$(BUILDDIR)/%.o:%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@norminette -R CheckForbiddenSourceHeader $< > /dev/null 				\
		&& $(P) "Build${GREEN}" "$(notdir $<)${RESET} " 						\
		|| $(P) "Build${RED}" "$(notdir $<)${RESET}"

$(LIBS):
	@mkdir -p $(BUILDDIR)
	@git submodule update --init 2>&1 > /dev/null	\
		&& $(P) "Git${GREEN}" "$@$(RESET)"	\
		|| $(P) "Git${RED}" "$@$(RESET)"
	@make -C $(LIBDIR)/$@ > /dev/null			\
		&& $(P) "Build${GREEN}" "$@$(RESET)"		\
		|| $(P) "Build${RED}" "$@$(RESET)"	

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(LIBDIR)/*/*.a

fclean: clean
	@rm -rf $(LIBDIR)/*/build
	@rm -rf $(LIBDIR)/*.a

	@rm -f $(NAME)
	@if [ "./" != "$(dir $(TARGET))" ]; then rm -rf $(dir $(TARGET)); fi

re: fclean all

debug: CFLAGS += -D DEBUG=1 -g -fsanitize=address
debug: re all
	@printf "$(RED)Compiled in debug / fsanitize=address mode!!!$(RESET)\n\n"

leaks: CFLAGS += -D DEBUG=2 -g
leaks: re all
	@printf "$(RED)Compiled in debug / leaks mode!!!$(RESET)\n\n"

thread: CFLAGS += -fsanitize=thread -g
thread: re all
	@printf "$(RED)Compiled in debug / thread mode!!!$(RESET)\n\n"

norm: $(SOURCES)
	@norminette -R CheckForbiddenSourceHeader $^ include 2>&1 > /dev/null && exit 0 || exit 1

.PHONY: CFLAGS all clean fclean re
