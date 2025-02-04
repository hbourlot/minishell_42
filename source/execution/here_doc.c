/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joralves <joralves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:06:50 by hbourlot          #+#    #+#             */
/*   Updated: 2025/02/04 09:51:56 by joralves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cleanup_and_exit(char *text, int error_code)
{
	if (text)
		free(text);
	return (error_code);
}

int	here_doc(int *pipe_id, char *limiter)
{
	char	*text;

	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		text = get_next_line(STDIN_FILENO);
		if (!text)
			return (cleanup_and_exit(NULL, -1));
		if (!ft_strcmp(limiter, text))
			break ;
		if (ft_strlen(text) == 0)
			return (cleanup_and_exit(text, -1));
		if (!ft_strchr(text, '\n') && !ft_strncmp(text, limiter, ft_strlen(text)
				- 1))
			return (cleanup_and_exit(text, -1));
		ft_putstr_fd(text, pipe_id[1]);
		free(text);
	}
	return (cleanup_and_exit(text, 0));
}

static void	handle_child_process(t_shell *data)
{
	int	i;

	i = 0;
	while (data->eof && data->eof[i])
	{
		if (here_doc(data->pipe_id, data->eof[i]) == -1)
		{
			close(data->pipe_id[1]);
			set_error_ex(1, "Failed", "here_doc", true);
			handle_error();
		}
		i++;
	}
	if (!data->command) // TODO: Still need to make sure about it
		write(1, "\n", 1);
	close(data->pipe_id[0]);
	close(data->pipe_id[1]);
	cleanup_shell(data);
	exit(EXIT_SUCCESS);
}

static int	handle_parent_process(t_shell *data)
{
	if (data->command)
		data->prev_fd = data->pipe_id[0];
	else
		close(data->pipe_id[0]);
	data->commands_ran += 1;
	close(data->pipe_id[1]);
	return (0);
}

int	run_eof(t_shell *data, pid_t *pid)
{
	if (pipe(data->pipe_id) == -1)
		perror("Pipe");
	signal(SIGINT, SIG_IGN);
	*pid = fork();
	if (*pid == -1)
		perror("Fork");
	if (*pid == 0)
	{
		restore_signals();
		handle_child_process(data);
	}
	if (handle_parent_process(data))
		return (-1);
	return (0);
}
