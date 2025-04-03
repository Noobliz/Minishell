/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:34:50 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:37:03 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

void	create_pipe(int new_pipe[2], t_cmd *tmp, t_data *data)
{
	if (tmp->next)
	{
		if (pipe(new_pipe) == -1)
			return (msg_error(data, "pipe", -1));
	}
}

void	update_pipe(t_cmd *current, int old_pipe[2], int new_pipe[2])
{
	if (current->next)
	{
		old_pipe[0] = new_pipe[0];
		old_pipe[1] = new_pipe[1];
	}
}

void	wait_for_kids(t_cmd *cmds, t_data *data)
{
	t_cmd	*tmp;
	int		status;

	tmp = cmds;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (!tmp->next)
		{
			if (WIFEXITED(status))
				data->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->last_exit_code = 128 + WTERMSIG(status);
		}
		tmp = tmp->next;
	}
}

void	check_files(t_cmd *current, int old_pipe[2], int new_pipe[2],
		t_data *data)
{
	if (current->infile == -1)
	{
		if (current->previous)
			close_all_pipes(old_pipe);
		if (current->next)
			close_all_pipes(new_pipe);
		free_exit(data, 1);
	}
	if (current->outfile == -1)
	{
		if (current->previous)
			close_all_pipes(old_pipe);
		if (current->next)
			close_all_pipes(new_pipe);
		free_exit(data, 1);
	}
}

void	builtin_in_fork(t_cmd *tmp, t_data *data)
{
	if (tmp->built_in >= 0)
	{
		if (built_in(tmp->built_in, tmp->argv, data->env_array, data)
			== -1)
		{
			close_fd_new(tmp, tmp->next);
			free_exit(data, 1);
		}
		close_fd_new(tmp, tmp->next);
		free_exit(data, 0);
	}
}
