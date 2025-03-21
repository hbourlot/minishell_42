# -- Colors and formatting
RED				=\033[1;31m
YELLOW 			= \033[1;33m
ORANGE 			= \033[1;38;5;214m
GREEN 			= \033[1;32m
CYAN 			= \033[1;36m
RESET 			= \033[0m

CC				= cc
CFLAGS			=  -g -Wall -Wextra -Werror #-pthread #-fsanitize=address,undefined 
LIB				= minishell.a
INCLUDE 		= include/
HEADER_MINI		= $(INCLUDE)minishell.h
HEADER_ERROR	= $(INCLUDE)error.h
HEADER_DEF		= $(INCLUDE)definitions.h
SRC_DIR 		= source/
BONUS_DIR 		= bonus/
OBJ_DIR 		= objects/
TOTAL_FILES		= $(shell echo $$(($(words $(SRC_FILES)) + 1)))
COMPILED_FILES	= 0
OS				= $(shell uname)
NAME			= minishell
C_FUNCTIONS		= parsing/syntax parsing/syntax_tokens parsing/strip_redirects parsing/replace_sqpa_tokens 			\
					parsing/command_token_execution	parsing/command_path_execution 									\
					parsing/syntax_quotes_matching																	\
					 																								\
					initialize/command initialize/command_aux initialize/shell initialize/redir_files 				\
					initialize/env_paths initialize/handle_expansion initialize/process_input_expanded 	 			\
					initialize/input_expansion initialize/hashmap initialize/hashmap_aux initialize/wildcard		\
					initialize/wildcard_aux																			\
																							             			\
					execution/parent	execution/utils																\
					execution/get_path execution/handle_folders execution/here_doc execution/child					\
					execution/run_commands execution/child_aux	execution/here_doc_aux 								\
																													\
					builtin/cd builtin/echo builtin/env builtin/exit builtin/export builtin/handler builtin/pwd     \
					builtin/unset																					\
																													\
					signals/signal      																			\
																													\
					utils/shell_cleanup utils/debug utils/debug1 utils/error_tools utils/error_parsing				\
					utils/error_initialize utils/error_execution utils/useful_functions utils/useful_functions2 utils/hashmap_free
# -L./ -lminishell
# VALGRIND		= valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND		= valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline.supp
LINK			= ./minishell.a -L./lib/library/ -lft -lreadline
SRC_FILES 		= $(addprefix $(SRC_DIR), $(C_FUNCTIONS:=.c))
OBJS_SRC 		= $(addprefix $(OBJ_DIR), $(SRC_FILES:%.c=%.o))

# -- INCLUDES LIBRARIES
LIBFT_LIB = ./lib/library/libft.a

ifeq ($(OS), Darwin)
	PRINT_CMD = printf
	MSG = "\r%100s\r[ $(COMPILED_FILES)/$(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))%% ] $(ORANGE)Compiling [$1] ... $(RESET)"
else
	PRINT_CMD = echo -n
	MSG = "\r%100s\r[ $(COMPILED_FILES)/$(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))% ] $(ORANGE)Compiling [$1]... $(RESET)"
endif

# Function to print the compilation message
define print_compile_msg
	$(eval COMPILED_FILES = $(shell echo $$(($(COMPILED_FILES) + 1))))
	@$(PRINT_CMD) $(MSG)
endef


.PHONY: 		all clean fclean re bonus

all:			$(NAME)

$(NAME):		$(LIBFT_LIB) $(LIB) $(HEADER_MINI) $(HEADER_DEF) $(HEADER_ERROR) main.o
				@$(CC) $(CFLAGS) main.o $(LINK) -o $@
				@echo "$(GREEN)Executable '$(RED)$(NAME)$(GREEN)' created successfully!$(RESET) ✅"

$(LIBFT_LIB):
				@make -s -C lib/library/

$(LIB):			$(OBJS_SRC)
				@ar rcs $@ $(OBJS_SRC)
				@echo "$(CYAN)library created successfully!$(RESET)"

$(OBJ_DIR)%.o:	%.c $(INCLUDE)
				@mkdir -p $(dir $@)
				$(call print_compile_msg, $<)
				@$(CC) $(CFLAGS) -c $< -I./$(INCLUDE) -o $@

main.o:			main.c $(INCLUDE)
				$(call print_compile_msg, $<)
				@$(CC) -c main.c $(CFLAGS) -I./$(INCLUDE) -o $@

clean:
				@printf "$(CYAN)Cleaning up *.o files...$(RESET)\b"
				@rm -rf $(OBJ_DIR)
				@rm -rf main.o
				@make clean -s -C ./lib/library
				@echo " $(GREEN)Cleaned successfully.$(RESET) ✅"

fclean: 		clean
				@printf "$(CYAN)Cleaning up '$(RED)$(NAME)$(CYAN)' and *.a files...$(RESET)\b"
				@rm -f $(LIB)
				@make fclean -s -C ./lib/library
				@rm -rf	$(NAME)
				@echo " $(GREEN)Cleaned successfully.$(RESET) ✅"

re: 			fclean all

bonus:			all

TEST =			test_minishell

$(TEST): 		$(LIBFT_LIB) $(LIB) $(HEADER_MINI)
				$(CC) $(LINK) -o test_minishell
				./test_minishell

t: $(TEST)

r:
	@make -s
	@./minishell

v:
	@make -s
	@$(VALGRIND) ./minishell



# 	{
#     leak readline
#     Memcheck:Leak
#     ...
#     fun:readline
# }
# {
#     leak add_history
#     Memcheck:Leak
#     ...
#     fun:add_history
# }
# {
#     leak readline
#     Memcheck:Leak
#     ...
#     fun:readline
# }
# {
#     leak add_history
#     Memcheck:Leak
#     ...
#     fun:add_history
# }
# {
#     still_reachable xrealloc
#     Memcheck:Leak
#     ...
#     fun:realloc
# }
# {
#     still_reachable rl_extend_line_buffer
#     Memcheck:Leak
#     ...
#     fun:rl_extend_line_buffer
# }
# {
#     still_reachable rl_insert_text
#     Memcheck:Leak
#     ...
#     fun:rl_insert_text
# }
# {
#     still_reachable rl_bracketed_paste_begin
#     Memcheck:Leak
#     ...
#     fun:rl_bracketed_paste_begin
# }
# {
#     still_reachable _rl_dispatch_subseq
#     Memcheck:Leak
#     ...
#     fun:_rl_dispatch_subseq
# }
# {
#     still_reachable readline_internal_char
#     Memcheck:Leak
#     ...
#     fun:readline_internal_char
# }
# {
#     readline_leak
#     Memcheck:Leak
#     ...
#     fun:malloc
#     obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.1
# }
