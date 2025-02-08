/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:05:21 by hbourlot          #+#    #+#             */
/*   Updated: 2025/02/07 15:15:45 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	split_command_input(t_shell *data, const char *delimiters[])
{
	sort_strings_by_length_desc((char **)delimiters);
	data->rl_splitted = split_by_multiple_tokens(data->rl, delimiters);
	if (!data->rl_splitted)
		return (ERROR);
	return (SUCCESS);
}

static bool	there_is_no_command(t_shell *data)
{
	const char	*tokens[] = {"||", "|", "&&", ">>", "<<", "<", ">", NULL};
	int			i;
	int			idx;
	int			match;

	i = 0;
	while (data->rl[i])
	{
		while (data->rl[i] == REP_SPACE)
			i++;
		match = find_string_match(&data->rl[i], tokens, &idx);
		if (data->rl[i] && match == -1)
			return (false);
		if (match != -1)
		{
			i += ft_strlen(tokens[idx]);
			if (!data->rl[i])
				break ;
		}
	}
	return (true);
}

static int	create_command_list(t_shell *data, const char *delimiters[])
{
	t_token	token_type;
	int		i;
	int		idx;
	char	*src;
	int		match_result;

	i = 0;
	src = data->rl;
	while (data->rl_splitted[i])
	{
		src = ft_strstr_any(src, delimiters);
		match_result = find_string_match(src, delimiters, &idx);
		if (match_result == CMP_OK)
		{
			token_type = get_t_token((char *)delimiters[idx],
					ft_strlen(delimiters[idx]));
			src += ft_strlen(delimiters[idx]);
		}
		else
			token_type = NO_TOKEN;
		if (add_command(&data->command, data->rl_splitted[i++], data,
				token_type) < 0)
			return (ERROR);
	}
	data->nbr_of_commands = i;
	return (SUCCESS);
}

static int	handle_eof(t_shell *data)
{
	const char	*eof_token[] = {"<<", NULL};

	if (initialize_eof(data->rl, &data->eof) < 0)
	{
		set_error_in(1, "\"EOF_HERE_DOC\"", __func__, true);
		return (-1);
	}
	strip_redirects(data->rl, eof_token);
	if (there_is_no_command(data))
		free_pointers(1, &data->rl);
	return (0);
}

int	init_command(t_shell *data)
{
	const char	*delimiters[] = {"||", "|", "&&", NULL};

	if (handle_eof(data))
		return (-1);
	if (data->rl && (split_command_input(data, delimiters) < 0
			|| create_command_list(data, delimiters) < 0))
		return (-1);
	return (0);
}
