/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbourlot <hbourlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:23:56 by hbourlot          #+#    #+#             */
/*   Updated: 2025/02/11 16:00:28 by hbourlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_wait_stats(int wait_status, int *status)
{
	int	sig;

	if (WIFEXITED(wait_status))
		*status = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
	{
		sig = WTERMSIG(wait_status);
		*status = 128 + sig;
		if (sig == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		else if (sig == SIGINT)
			write(1, "\n", 1);
	}
}

void	set_last_status(t_shell *data)
{
	int		status;
	int		wait_status;
	pid_t	prev_pid;
	int		i;

	i = 0;
	prev_pid = 0;
	wait_status = 0;
	status = 0;
	if (data->commands_ran == 0)
		return ;
	while (i < data->commands_ran)
	{
		data->pid = waitpid(-1, &wait_status, 0);
		if (data->pid > prev_pid)
			handle_wait_stats(wait_status, &status);
		prev_pid = data->pid;
		i++;
	}
	if (data->last_cmd_executed->settings.is_builtin
		&& data->last_cmd_executed->settings.builtin_id != ECHO)
		return ;
	data->exit_status = status;
}

int	do_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid < 0)
		return (-1);
	return (0);
}

void	here_doc_fail(t_shell *data, t_file *current)
{
	int	size;

	size = ft_strlen(current->eof);
	get_error_context()->exit = true;
	truncate_range(current->eof, size - 1, 1);
	ft_printf_error("\nbash: warning: here-document at line ");
	ft_printf_error("%d delimited by end-of-file (wanted `%s')\n",
		data->nbr_of_lines, current->eof);
}
