/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useful_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:33:57 by hbourlot          #+#    #+#             */
/*   Updated: 2025/03/05 17:32:56 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_hashnode	*sorted_insert(t_hashnode *sorted, t_hashnode *new_node)
{
	t_hashnode	*current;

	if (!new_node)
		return (sorted);
	if (!sorted || ft_strcmp(new_node->key, sorted->key) < 0)
	{
		new_node->next = sorted;
		return (new_node);
	}
	current = sorted;
	while (current->next && ft_strcmp(current->next->key, new_node->key) < 0)
		current = current->next;
	new_node->next = current->next;
	current->next = new_node;
	return (sorted);
}

void	insertion_sort(t_hashnode **head)
{
	t_hashnode	*sorted;
	t_hashnode	*current;
	t_hashnode	*next;

	if (!head || !*head || !(*head)->next)
		return ;
	sorted = NULL;
	current = *head;
	while (current)
	{
		next = current->next;
		current->next = NULL;
		sorted = sorted_insert(sorted, current);
		current = next;
	}
	*head = sorted;
}

t_token	get_t_token(char *src, size_t size)
{
	if (ft_strncmp(src, "|", size) == CMP_OK)
		return (PIPE_SINGLE);
	else if (ft_strncmp(src, "||", size) == CMP_OK)
		return (PIPE_DOUBLE);
	else if (ft_strncmp(src, "&&", size) == CMP_OK)
		return (AND_DOUBLE);
	else if (ft_strncmp(src, "<", size) == CMP_OK)
		return (REDIRECT_LEFT_SINGLE);
	else if (ft_strncmp(src, "<<", size) == CMP_OK)
		return (REDIRECT_LEFT_DOUBLE);
	else if (ft_strncmp(src, ">", size) == CMP_OK)
		return (REDIRECT_RIGHT_SINGLE);
	else if (ft_strncmp(src, ">>", size) == CMP_OK)
		return (REDIRECT_RIGHT_DOUBLE);
	return (NO_TOKEN);
}

static void	toggle_quotes(char *src, bool *in_quotes, int *i)
{
	while (src[*i] && (src[*i] == REP_SQ || src[*i] == REP_DQ))
	{
		*in_quotes = !*in_quotes;
		(*i)++;
	}
}

void	get_redir_segment(char *src, int *start, int *end,
		int redir_length)
{
	int			i;
	int			idx;
	bool		in_quotes;
	const char	*tokens[] = {"||", "|", "&&", ">>", "<<", "<", ">", NULL};

	i = 0;
	in_quotes = false;
	if (!src)
		return ;
	i += redir_length;
	while (src[i] && src[i] == REP_SPACE)
		i++;
	toggle_quotes(src, &in_quotes, &i);
	*start = i;
	while (src[i] && src[i] != REP_SPACE)
	{
		if (src[i] == REP_SQ || src[i] == REP_DQ)
			in_quotes = !in_quotes;
		if (!in_quotes && find_string_match(&src[i], tokens, &idx) == OK)
			break ;
		i++;
	}
	*end = i;
}
