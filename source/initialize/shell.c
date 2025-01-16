/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:31:29 by hbourlot          #+#    #+#             */
/*   Updated: 2025/01/20 16:09:39 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static bool verify_and_prepare_input(t_shell *data)
{

	if (data->readline && *data->readline)
		add_history(data->readline);
	if (ft_strlen(data->readline) == 0 || all_same_char(data->readline, ' '))
	{
		free_pointers(1, &data->readline);
		printf("\n");
		return (false);
	}
	if (parsing_syntax(data->readline) == -1)
		return (false);
	if (init_command(data->readline) == -1)
		return (false);
	return (true);
}

int	main_shell_loop(t_shell *data)
{
	char	**args;

	while (true)
	{
		data->readline = readline("[Chitãozinho&Xororó@localhost ~]$ ");
		if (!data->readline || ft_strcmp("exit", data->readline) == CMP_OK)
            return (printf("exit\n"), 0);
		if (verify_and_prepare_input(data) == false)
			handle_error();
		if (data->command)
			run_commands(data);
		refresh_shell_data(data);
	}
	return (0);
}

t_shell	*init_shell(int argc, char *argv[], char *envp[])
{
	t_shell *data;

	data = get_shell();
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	if (initialize_environment_paths(data))
	{
		cleanup_shell(data);
		exit(EXIT_FAILURE); // TODO: Add a custom message ??
	}
	return (data);
}

/// @brief Provides access to a singleton instance of the shell structure.
/// @return A pointer to the singleton `t_shell` instance.
t_shell	*get_shell(void)
{
	static t_shell	data;

	return (&data);
}