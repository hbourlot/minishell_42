/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:18:25 by hbourlot          #+#    #+#             */
/*   Updated: 2024/12/24 20:37:27 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_command_input(t_shell *data)
{
	t_cmd	*tmp;
	int		i;

	i = 1;
	tmp = data->command;
	while (tmp)
	{
		printf("Command->pre_command [%d]: %s\n", i++, tmp->input);
		tmp = tmp->next;
	}
}

void	debug_command_args(t_shell *data)
{
	t_cmd	*tmp;
	int		i;
	int		j;

	i = 1;
	tmp = data->command;
	while (tmp)
	{
		j = 0;
		if (tmp->args)
		{
			while (tmp->args[j])
			{
				printf("Command->args [%d][%d]: %s\n", i, j + 1, tmp->args[j]);
				j++;
			}
		}
		else
		{
			printf("args == NULL\n");
			break ;
		}
		i++;
		tmp = tmp->next;
	}
}

void	debug_command_path(t_shell *data)
{
	int		i;
	t_cmd	*tmp;

	i = 1;
	tmp = data->command;
	while (tmp)
	{
		if (tmp->path)
			printf("Command->path [%d]: %s\n", i, tmp->path);
		else
			printf("Command->path [%d]: has no path", i);
		i++;
		tmp = tmp->next;
	}
}
// void	error_msg(void)
// {
// 	ft_putstr_fd("Error\n", 2);
// }

static void debug_files(t_file *file_list)
{
    while (file_list)
    {
        printf("File:\n");
        if (file_list->read)
            printf("  Read: %s\n", file_list->read);
        if (file_list->write)
            printf("  Write: %s\n", file_list->write);
        printf("  Delimiter: %d\n", file_list->redirect);
        file_list = file_list->next;
    }
}
void debug_command_file_list(t_shell *data)
{
    int 	cmd_num;
	t_cmd 	*tmp;
	t_file	*tmp_files;

	cmd_num = 1;
	tmp = data->command;
    while (tmp)
    {
        printf("Command %d:\n", cmd_num++);
        if (tmp->input)
            printf("  Input: %s\n", tmp->input);
		if (tmp->file_list)
			debug_files(tmp->file_list);
        else
            printf("  No files associated.\n");
        tmp = tmp->next;
    }
}

void debug_input_splitted(t_shell *data)
{
	int		i;
	char	**tmp_split;

	i = 1;
	tmp_split = data->input_splitted;
	while (*tmp_split)
	{
		printf("Data->input_splitted [%d]: %s\n", i++, *tmp_split);
		tmp_split++;
	}
}
