# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbouma <jbouma@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/10/10 14:09:40 by jbouma        #+#    #+#                  #
#    Updated: 2023/07/25 01:21:58 by jensbouma     ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# Program Name(s)
NAME		=	philo

# Set build directories
BUILDDIR	= build
TARGET		= $<

# Compiler Settings
CC 			:= cc
CFLAGS		+= -Werror -Wall -Wextra
# CFLAGS		+= -fsanitize=thread -g
# CFLAGS		+= -g
# CFLAGS		+= -O3 -flto -march=native -mtune=native


# Libraries
LIBDIR		= lib
LIBS		= libvector

LIBS_A		= $(addsuffix /*.a, $(addprefix $(LIBDIR)/, $(LIBS)))
LIBS_INC	= $(addsuffix /include, $(addprefix $(LIBDIR)/, $(LIBS)))

# Headers
INC 		= -I include -I$(LIBS_INC)


# Sources
SRCDIR		= 	src
FILES		= 	input.c			\
				main.c			\
				msg.c			\
				philo.c			\
				tools/error.c	\
				tools/mem.c		\
				tools/time.c	\
				tools/value.c	\
				tools/string.c
	
SOURCES		=	${addprefix $(SRCDIR)/, $(FILES)}

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
	RETURN		:= "\033[0K\n"
P				:= printf "%-25.25s%s\n"
P_OK			= $(P) "$@${GREEN}" "Norm OK" "${RESET}"
P_KO			= $(P) "$@${RED}" "Norm KO" "${RESET}"

# Rules
all: $(NAME)
	@if [ ! -d $(dir $(TARGET)) ]; then mkdir $(dir $(TARGET)); fi
	@$(CC) $(CFLAGS) $(INC) $(HEADERS) $(OBJECTS) $(LIBS_A) $(GLFW) -o $(TARGET)	\
		&& ($(P) "Executable $(GREEN)" "$< Created $(RESET)" && exit 0)						\
		|| ($(P) "Executable $(RED)" "$< Compile error $(RESET)" && exit 1)
	@$(P) "Flags $(YELLOW)" "$(CFLAGS) $(RESET)"
	@make norm 2> /dev/null && ($(P) "Norminette$(GREEN)" "OK$(RESET)" && printf "\n🙏 $(GREEN)Complete $(RESET)\n") || $(P) "Norminette$(RED)" "KO$(RESET)"

$(BUILDDIR)/%.o:%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@norminette -R CheckForbiddenSourceHeader $< > /dev/null 				\
		&& $(P) "Build${GREEN}" "$(notdir $<)${RESET}" 						\
		|| $(P) "Build${RED}" "$(notdir $<)${RESET}"

$(LIBS):
	@mkdir -p $(BUILDDIR)
	@git submodule update --init 2>&1				\
		&& $(P) "Submodule${GREEN}" "$@$(RESET)"	\
		|| $(P) "Submodule${RED}" "$@$(RESET)"
	@make -C $(LIBDIR)/$@ 2>&1 > /dev/null			\
		&& $(P) "Build${GREEN}" "$@$(RESET)"		\
		|| $(P) "Build${RED}" "$@$(RESET)"	

$(NAME): $(LIBS) $(OBJECTS)
	@make norm 2> /dev/null && $(P_OK) || { $(P_KO);}

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(LIBDIR)/*/*.a

fclean: clean
	@rm -rf $(LIBDIR)/*/build
	@rm -rf $(LIBDIR)/*.a

	@rm -f $(NAME)
	@if [ "./" != "$(dir $(TARGET))" ]; then rm -rf $(dir $(TARGET)); fi

re: fclean all

debug: CFLAGS += -g -fsanitize=address -D DEBUG=1
debug: all
	@printf "$(RED)Compiled in debug / fsanitize=adress mode!!!$(RESET)\n\n"

leaks: CFLAGS += -D DEBUG=2
leaks: all
	@printf "$(RED)Compiled in debug / leaks mode!!!$(RESET)\n\n"

norm: $(SOURCES)
ifneq ($(UNAME_S),Linux)
	@norminette -R CheckForbiddenSourceHeader $^ include 2>&1 > /dev/null && exit 0 || exit 1
else
	@echo "Norminette not available on this machine" && exit 0
endif

.PHONY: CFLAGS all clean fclean re
