/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:06:50 by hbourlot          #+#    #+#             */
/*   Updated: 2025/01/23 23:33:32 by hbourlot         ###   ########.fr       */
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

static void	handle_child_process(t_shell *data, int *pipe_id, int *prev_fd)
{
	int	i;

	i = 0;
	while (data->eof && data->eof[i])
	{
		for (int h = 0; data->eof[h]; h++)
		{
			ft_printf_error("eof[%d]:", h);;
			for (int j = 0; data->eof[i][j]; j++)
				ft_printf_error("(%c-%d)", data->eof[i][j], data->eof[i][j]);
			ft_printf_error("\n");
		}
		if (here_doc(pipe_id, get_shell()->eof[i]) == -1)
		{
			close(pipe_id[1]);
			set_error_execution(1, "Failed", "here_doc", true);
			handle_error();
		}
		i++;
	}
	if (!data->command) // TODO: Still need to make sure about it
		write(1, "\n", 1);
	close(pipe_id[0]);
	close(pipe_id[1]);
	cleanup_shell(data);
	exit(EXIT_SUCCESS);
}

static void	handle_parent_process(t_shell *data, int *pipe_id, pid_t *pid)
{
	pid_t	result;
	int		wait_status;
	
	close(pipe_id[0]);
	result = waitpid(*pid, &wait_status, 0);
	if (WIFEXITED(wait_status))
		get_shell()->exit_status = WEXITSTATUS(wait_status);
	close(pipe_id[1]);
}

int	run_eof(t_shell *data, int *pipe_id, int *prev_fd, pid_t *pid)
{
	if (pipe(pipe_id) == -1)
		perror("Pipe");
	*pid = fork();
	if (*pid == -1)
		perror("Fork");
	if (*pid == 0)
		handle_child_process(data, pipe_id, prev_fd);
	handle_parent_process(data, pipe_id, pid);
	return (0);
}
