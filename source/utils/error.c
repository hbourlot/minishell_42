/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:10:52 by hbourlot          #+#    #+#             */
/*   Updated: 2024/12/20 18:10:55 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Provides a static error context shared
///				across functions. This allows centralizing
///				error handling in one place.
/// @return A pointer to the static t_error structure.
static t_error	*error_context(void)
{
	static t_error	error;

	return (&error);
}

/// @brief Resets the error context by clearing its fields.
/// @param error Pointer to the error context to reset.
static void	reset_error(t_error *error)
{
	error->code = 0;
	error->token = NULL;
	error->message = NULL;
	error->function = NULL;
}

/// @brief Handles and prints the current error context.
///        	It displays syntax errors or generic errors
///				based on the context.
/// @return The current error code.
int	handle_error(void)
{
	t_error	*error;
	int		code;

	error = error_context();
	if (error->token)
	{
		ft_putstr_fd(SYNTAX_ERROR_MSG, 2);
		ft_putstr_fd(error->token, 2);
		ft_putstr_fd("'\n", 2);
	}
	else if (error->message)
	{
		fprintf(stderr, "Error: %s in function: %s\n", error->message,
			error->function);
		reset_error(error);
	}
	code = error->code;
	reset_error(error);
	return (error->code);
}

/// @brief Sets an error in the error context with the provided details.
/// @param code The error code to set.
/// @param message A string describing the error.
/// @param token The problematic token (if applicable).
/// @param function The name of the function where the error occurred.
void	set_error(int code, const char *message, char *token,
		const char *function)
{
	t_error	*error;

	error = error_context();
	error->code = code;
	error->token = token;
	error->message = (char *)message;
	error->function = (char *)function;
}
