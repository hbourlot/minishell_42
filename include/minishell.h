/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:50:06 by hbourlot          #+#    #+#             */
/*   Updated: 2024/12/25 12:20:55 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/library/inc/libft.h"
# include "definitions.h"


// ************************************************************************
// **						Parsing Functions							 **
// ************************************************************************

int			parsing_syntax(char *input);
int 		parsing_and_strip_redirects(char **input, const char *redirects[]);


// ************************************************************************
// **						Create Functions							 **
// ************************************************************************

// t_cmd		*create_command_list(char **input_splitted);

// ************************************************************************
// **						Initialize Functions						 **
// ************************************************************************

t_shell 	*get_shell();
t_shell		*init_shell(int argc, char *argv[], char *envp[]);
int			init_program(t_shell *data);
int			init_command(char *input);
int 		init_file_list(char *input, const char *redirects[], t_file **file_list);

// ************************************************************************
// **						Proccess Functions						 	 **
// ************************************************************************

char		**get_command_args(char *argv);
void		run_commands(t_shell *data);
char 		*get_path(char *input, char **env_paths);
int			open_folder(char *file, t_cmd *command, bool here_doc);
int			do_dup2(t_cmd *command, int *pipe_id, int *prev_fd);

// ************************************************************************
// **						Free Functions							 	 **
// ************************************************************************

void		cleanup_shell(t_shell*data);
void		refresh_shell_data(t_shell *data);
void 		free_files(t_file *file_list);


// ************************************************************************
// **						Utils Functions							 	 **
// ************************************************************************

void		debug_command_input(t_shell *data);
void		debug_command_args(t_shell *data);
void		debug_command_path(t_shell *data);
void 		debug_command_file_list(t_shell *data);
void 		debug_input_splitted(t_shell *data);

// void 		error_msg(void);

// ************************************************************************
// **						Error Functions							 	 **
// ************************************************************************

int 		handle_error(void);
void 		set_error(int code, const char *message, char *token, const char *function);

#endif
