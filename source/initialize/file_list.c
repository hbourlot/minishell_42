/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:31:14 by hbourlot          #+#    #+#             */
/*   Updated: 2025/02/11 10:40:04 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_file(char *input, int *position, t_token token, t_file **rf)
{
	t_file	*new;
	t_file	*current;
	char	*src;

	new = ft_calloc(1, sizeof(t_file));
	if (!new)
		return (-1);
	src = ft_substr(input, position[0], position[1] - position[0]);
	if (!src)
		return (-1);
	new->redirect = token;
	if (token == REDIRECT_LEFT_SINGLE)
		new->read = src;
	else if (token == REDIRECT_RIGHT_SINGLE || token == REDIRECT_RIGHT_DOUBLE)
		new->write = src;
	current = *rf;
	if (!current)
		*rf = new;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

int	initialize_file_list(char *input, const char *redirects[], t_file **rf)
{
	int		idx;
	int		position[2];
	t_token	token;
	bool	in_quotes;

	in_quotes = false;
	while (input && *input)
	{
		if (*input && (*input == REP_DQ
				|| *input == REP_SQ))
			in_quotes = !in_quotes;
		else if (!in_quotes && find_string_match(input, redirects, &idx) == OK)
		{
			token = get_t_token(input, ft_strlen(redirects[idx]));
			get_redirect_complement(input, &position[0], &position[1],
				ft_strlen(redirects[idx]));
			if (add_file(input, position, token, rf) < 0)
				return (-1);
		}
		input++;
	}
	return (0);
}
