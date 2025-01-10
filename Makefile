# -- Colors and formatting
RED				=\033[1;31m
YELLOW 			= \033[1;33m
ORANGE 			= \033[1;38;5;214m
GREEN 			= \033[1;32m
CYAN 			= \033[1;36m
RESET 			= \033[0m

CC				= cc
CFLAGS			= -g #-Wall -Wextra #-Werror #-fsanitize=thread -g -pthread
LIB				= minishell.a
INCLUDE 		= include/
HEADER_FILE		= $(INCLUDE)minishell.h
SRC_DIR 		= source/
BONUS_DIR 		= bonus/
OBJ_DIR 		= objects/
TOTAL_FILES		= $(words $(OBJS_SRC))
COMPILED_FILES	= 0
OS				= $(shell uname)
MSG_MAC 		= "\r%100s\r[ $(COMPILED_FILES)/$(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))%% ] $(ORANGE)Compiling... $<... $(RESET)"
MSG_LINUX 		= "\r%100s\r[ $(COMPILED_FILES)/$(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))% ] $(ORANGE)Compiling... $<... $(RESET)"
NAME			= minishell
C_FUNCTIONS		= parsing/syntax parsing/syntax_pipe_redirects parsing/strip_redirects parsing/command_execution parsing/replace_sq_tokens															\
					initialize/command initialize/command_tools initialize/shell initialize/file_list initialize/here_doc initialize/env_paths								\
					execution/argument_parser execution/run_commands execution/run_command_tools execution/get_path execution/handle_folders execution/run_here_doc	execution/expand_args		\
					utils/shell_cleanup utils/debug utils/debug1 utils/error_tools utils/error_parsing utils/error_initialize utils/error_execution utils/useful_functions
# -L./ -lminishell
# VALGRIND		= valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND		= valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline.supp
LINK			= ./minishell.a -L./lib/library/ -lft -lreadline
SRC_FILES 		= $(addprefix $(SRC_DIR), $(C_FUNCTIONS:=.c))
OBJS_SRC 		= $(addprefix $(OBJ_DIR), $(SRC_FILES:%.c=%.o))

# -- INCLUDES LIBRARIES
LIBFT_LIB = ./lib/library/libft.a

.PHONY: 		all clean fclean re bonus

all:			$(NAME)

$(NAME):		$(LIBFT_LIB) $(LIB) main.o $(HEADER_FILE)
				@$(CC) $(CFLAGS) main.o $(LINK) -o $@
				@echo "$(GREEN)Executable '$(NAME)' created successfully!$(RESET) ✅"

$(LIBFT_LIB):
				@make -s -C lib/library/

$(LIB):			$(OBJS_SRC)
				@ar rcs $@ $(OBJS_SRC)
				@echo "$(CYAN)library created successfully!$(RESET)"

$(OBJ_DIR)%.o:	%.c $(INCLUDE)
				@mkdir -p $(dir $@)
				$(eval COMPILED_FILES = $(shell echo $$(($(COMPILED_FILES) + 1))))
ifeq ($(OS), Darwin)
				@printf $(MSG_MAC)
else
				@echo -n $(MSG_LINUX)
endif
				@$(CC) $(CFLAGS) -c $< -I./$(INCLUDE) -o $@

main.o:			main.c $(INCLUDE)
				@$(CC) -c main.c $(CFLAGS) -I./$(INCLUDE) -o $@

clean:
				@printf "$(CYAN)Cleaning up *.o files...$(RESET)\b"
				@rm -rf $(OBJ_DIR)
				@rm -rf main.o
				@make clean -s -C ./lib/library
				@echo " $(GREEN)Cleaned successfully.$(RESET) ✅"

fclean: 		clean
				@printf "$(CYAN)Cleaning up '$(NAME)' and *.a files...$(RESET)\b"
				@rm -f $(LIB)
				@make fclean -s -C ./lib/library
				@rm -rf	$(NAME)
				@echo " $(GREEN)Cleaned successfully.$(RESET) ✅"

re: 			fclean all

r:
	@make -s
	@./minishell

v:
	@make -s
	@$(VALGRIND) ./minishell