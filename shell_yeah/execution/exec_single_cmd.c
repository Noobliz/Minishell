/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:44:58 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:45:44 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static void	dup_files(t_cmd *tmp, t_data *data)
{
	if (tmp->infile >= 0)
	{
		if (dup2(tmp->infile, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close_fd_new(tmp, tmp->next);
			free_exit(data, 1);
		}
	}
	if (tmp->outfile >= 0)
	{
		if (dup2(tmp->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close_fd_new(tmp, tmp->next);
			free_exit(data, 1);
		}
	}
}

void	exec_single_cmd(t_cmd *tmp, t_data *data)
{
	tmp->pid = fork();
	if (tmp->pid == -1)
		return (msg_error(data, "fork", -1));
	if (tmp->pid == 0)
	{
		dup_files(tmp, data);
		close_fd_new(tmp, tmp->next);
		if (tmp->argv && execve(tmp->argv[0], tmp->argv,
				data->env_array) == -1)
		{
			perror(tmp->cmd);
			free_exit(data, 127);
		}
	}
	wait_for_kids(tmp, data);
}
