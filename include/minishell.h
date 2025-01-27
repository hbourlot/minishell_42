/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joralves <joralves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:50:06 by hbourlot          #+#    #+#             */
/*   Updated: 2025/01/25 18:03:15 by joralves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _DEFAULT_SOURCE
# include "../lib/library/inc/libft.h"
# include "definitions.h"
# include "error.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// ************************************************************************
// **						Parsing Functions								**
// ************************************************************************

int			parsing_syntax(char *input);
bool		is_valid_pipe_tokens(char *source);
void		identify_and_replace_sq_tokens(char **input);
bool		is_valid_file_and_here_doc_tokens(char *source);
int			validate_file_read_execution(t_file *redir_files);
int			validate_command_path_access(char *command_path);
void		strip_redirects(char *input, const char *redirects[]);

// ************************************************************************
// **						Initialize Functions							**
// ************************************************************************

t_shell		*get_shell(void);
int			init_command(char *input);
int			main_shell_loop(t_shell *data);
int			initialize_environment_paths(t_shell *data);
t_shell		*init_shell(int argc, char *argv[], char *envp[]);
char		**tokenize_element(char *element);
char		*process_variables(char *cmd_token);
char		**tokenize_bash_variables(char *src);
char		**process_command_input(char *input);
int			initialize_eof(char *data_readline, char ***data_eof);
int			add_command(t_cmd **command, char *readline_splitted, t_shell *data,
				t_token token_type);
int			initialize_file_list(char *input, const char *redirects[],
				t_file **redir_files);
t_hashmap	*create_map(void);
int			hashmap_insert(t_hashmap *map, char *key, char *value);
char		*hashmap_search(t_hashmap *map, char *key);
void		hashmap_delete(t_hashmap *map, char *key);
void		hashmap_display(t_hashmap *map);
void		hashmap_free(t_hashmap *map);
int			hashmap_to_env_array(t_shell *data, t_hashmap *map);
int			import_env_to_hashmap(t_hashmap *map, char *envp[]);
int			update_envp_and_envpath(t_shell *data);

// ************************************************************************
// **						Execution Functions								**
// ************************************************************************

void		run_commands(t_shell *data);
int			here_doc(int *pipe_id, char *limiter);
char		*get_path(char *input, char **env_paths);
int			open_folder(char *file, t_cmd *command, bool here_doc);
int			do_dup2(int *fd_in, int *fd_out, int *pipe_id, int *prev_fd);
int			run_eof(t_shell *data, int *pipe_id, int *prev_fd, pid_t *pid);
int			open_folders_safety(int *fd_in, int *fd_out, t_file *redir_files);
void		child_process(t_shell *data, t_cmd *command, int *pipe_id,
				int *prev_fd);

// ************************************************************************
// **						Free Functions									**
// ************************************************************************

void		cleanup_shell(t_shell *data);
void		free_files(t_file *file_list);
void		refresh_shell_data(t_shell *data);

// ************************************************************************
// **						Utils Functions									**
// ************************************************************************

void		skip_spaces(char **src);
void		debug_command_args(t_shell *data);
void		debug_command_path(t_shell *data);
void		debug_command_input(t_shell *data);
void		debug_input_splitted(t_shell *data);
t_token		get_t_token(char *src, size_t size);
void		debug_command_file_list(t_shell *data);
void		skip_character_diff(char **src, char c);
void		restore_original_characters(char **src);
void		skip_character_by_idx(char *src, char c, int *i);
void		replace_characters(char **src, char to_take, char to_put);
void		restore_signals(void);
void		setup_signals(void);

// ************************************************************************
// **						BuiltIn											**
// ************************************************************************

int			builtin_cd(t_shell *data,char **command_args);
void		check_if_is_builtin(t_shell *data, t_cmd *last_node);

#endif
