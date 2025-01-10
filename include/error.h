/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:35:41 by hbourlot          #+#    #+#             */
/*   Updated: 2025/01/03 11:07:46 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

#ifndef ERROR_H
#define ERROR_H


typedef struct s_execution
{
	bool					is_initialized;
	char					*msg;
	char					*folder;

}		t_execution;


typedef struct s_parsing
{
	bool					is_initialized;
	char 					*syntax;
	char					*token;
	char					*function;
}			t_parsing;

typedef struct s_initialize
{
	bool					is_initialized;
	char					*msg;
	char					*function;
}			t_initialize;

typedef struct s_error
{
	struct s_parsing		parsing;
	struct s_initialize		initialize;
	struct s_execution		execution;
	int						code;
	bool					exit;	
}			t_error;

// ************************************************************************
// **						Error Functions							 	 **
// ************************************************************************

t_error		*get_error_context(void);
void		reset_error_context(t_error *error);
int			handle_error(void);


int			handle_error_parsing(void);
void 		set_error_parsing(int code, char *syntax, char *token, const char *function);

int			handle_error_initialize(void);
void		set_error_initialize(int code, char *msg, const char *function, bool exit);

int			handle_error_execution(void);
void		set_error_execution(int code, char *msg, const char *folder, bool exit);

#endif
