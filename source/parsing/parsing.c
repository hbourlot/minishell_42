/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 18:41:52 by hbourlot          #+#    #+#             */
/*   Updated: 2024/12/20 16:34:20 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Checks if the sequence of tokens in the input string is valid.
///        Ensures that no invalid sequences like 
///        "||", "| >", or "| end-of-input" occur.
/// @param input The input string to validate.
/// @param tokens The array of token strings to check for in the input.
/// @return True if the token sequence is valid; false otherwise.
static bool	is_valid_token_sequence(char *input, const char *tokens[])
{
	size_t	len;
	int		idx;
	char	*source;

	source = input;
	while (source)
	{
		source = ft_strstr_any(source, tokens);
		if (source && ft_strcmps(source, tokens, &idx) == CMP_OK)
		{
			len = ft_strlen(tokens[idx]);
			source += len;
			while (*source && *source == ' ')
				source++;
			if (!*source || ft_strcmps(source, tokens, &idx) == CMP_OK)
			{
				if (!*source)
					return (set_error(2, SYNTAX_ERROR_MSG, "newline", __func__),
						false);
				return (set_error(2, SYNTAX_ERROR_MSG, (char *)tokens[idx],
						__func__), false);
			}
		}
	}
	return (true);
}

/// @brief Parses the input string to validate its syntax.
///        Specifically checks for invalid token sequences
///         and reports syntax errors.
/// @param input The input string to parse.
/// @return 0 if the input is valid; -1 if a syntax error is detected.
int	parsing_input(char *input)
{
	const char	*tokens[] = {"|", "||", "&&", ">", ">>", "<", "<<", NULL};

	sort_strings_by_length_desc((char **)tokens);
	if (is_valid_token_sequence(input, tokens) == false)
		return (-1);
	return (0);
}


// if (is_delimiters_together(input, delimiters))
// {
//     printf("bash: syntax error near unexpected token \n");
// 	return (-1);
// }
/*
	TODO: Need to add a checker if after redirect there's at least one character different than space
*/






// static int	match_delimiter(char *input, const char *delimiters[])
// {
// 	int	i;

// 	i = 0;
// 	while (delimiters[i])
// 	{
// 		if (!ft_strncmp(input, delimiters[i], ft_strlen(delimiters[i])))
// 			return (i);
// 		i++;
// 	}
// 	return (-1);
// }

// static bool	is_delimiters_together(char *input, const char *delimiters[])
// {
// 	int	first_match;
// 	int	second_match;

// 	if (!input || !*input)
// 		return (false);
// 	first_match = match_delimiter(input, delimiters);
// 	if (first_match != -1)
// 	{
// 		input += ft_strlen(delimiters[first_match]);
// 		while (*input == ' ')
// 			input++;
// 		second_match = match_delimiter(input, delimiters);
// 		if (second_match != -1)
// 			return (true);
// 	}
// 	return (is_delimiters_together(input + 1, delimiters));
// }