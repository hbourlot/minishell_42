/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joralves <joralves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:11:13 by joralves          #+#    #+#             */
/*   Updated: 2025/02/21 18:25:22 by joralves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	duplicate(t_hashmap *map, t_hashnode **temp)
{
	int			idx;
	t_hashnode	*new_node;
	t_hashnode	*current;

	idx = -1;
	while (++idx < HASHMAP_SIZE)
	{
		current = map->slots[idx];
		while (current)
		{
			new_node = malloc(sizeof(t_hashnode));
			if (!new_node)
				return (ERROR);
			new_node->key = ft_strdup(current->key);
			if (!new_node->key)
				return (free(new_node), ERROR);
			new_node->value = ft_strdup(current->value);
			if (!new_node->value && current->value)
				return (free(new_node->key), free(new_node), ERROR);
			new_node->next = *temp;
			*temp = new_node;
			current = current->next;
		}
	}
	return (0);
}

static void	print_key_value_sorted(t_hashnode **temp, int fd)
{
	t_hashnode	*current;

	insertion_sort(temp);
	current = *temp;
	while (current)
	{
		ft_printf_fd(fd, "declare -x %s", current->key);
		if (current->value)
			ft_printf_fd(fd, "=\"%s\"", current->value);
		ft_printf_fd(fd, "\n");
		current = current->next;
	}
}

static int	is_valid_key(t_shell *data, char *temp_key, char *str)
{
	int		i;
	bool	valid;

	valid = true;
	i = -1;
	while (temp_key && temp_key[++i])
	{
		if ((!ft_isalnum(temp_key[i]) && temp_key[i] != '_')
			|| ft_isdigit(temp_key[0]))
		{
			valid = false;
			break ;
		}
	}
	if (valid == false)
	{
		ft_printf_fd(2, "bash: export: '%s': not a valid identifier\n", str);
		data->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	add_new_variable_on_hashmap(t_shell *data, char *command_arg)
{
	char	*value;
	char	*temp_key;
	int		len;

	value = ft_strchr(command_arg, '=');
	if (!value)
	{
		if (is_valid_key(data, command_arg, command_arg))
			return (1);
		if (hashmap_insert(data->map, command_arg, NULL) == ERROR)
			return (ERROR);
		return (SUCCESS);
	}
	len = value - command_arg;
	if (len == 0)
		len++;
	temp_key = ft_substr(command_arg, 0, len);
	if (!temp_key)
		return (ERROR);
	if (is_valid_key(data, temp_key, command_arg))
		return (free(temp_key), 1);
	if (hashmap_insert(data->map, temp_key, value + 1) == ERROR)
		return (free(temp_key), ERROR);
	data->exit_status = 0;
	return (free(temp_key), 0);
}

/// @brief Handles the `export` command.
/// @param data The shell structure.
/// @param command_args The arguments array.
/// @details Prints variables if no args are given. Otherwise, adds new ones
///          and updates the environment. Returns 0 or ERROR.
int	builtin_export(t_shell *data, t_cmd *command, int fd)
{
	int			idx;
	int			length;
	t_hashnode	*temp;

	temp = NULL;
	idx = 1;
	length = array_length(command->args);
	if (length == 1)
	{
		if (duplicate(data->map, &temp) == ERROR)
			return (hashnode_free(temp), ERROR);
		print_key_value_sorted(&temp, fd);
		hashnode_free(temp);
		data->exit_status = 0;
		return (0);
	}
	while (idx < length)
	{
		if (add_new_variable_on_hashmap(data, command->args[idx]) == ERROR)
			return (ERROR);
		idx++;
	}
	update_envp_and_envpath(data);
	return (0);
}
