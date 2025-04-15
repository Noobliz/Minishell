/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:40:39 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/10 17:37:12 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static void	dup_in(t_cmd *tmp, t_data *data, int old_pipe[2])
{
	if (tmp->infile >= 0)
	{
		if (dup2(tmp->infile, STDIN_FILENO) == -1)
		{
			perror("dup2");
			free_exit(data, 1);
		}
		if (tmp->previous)
			close_all_pipes(old_pipe);
		close(tmp->infile);
	}
	else if (tmp->previous)
	{
		if (old_pipe[0] >= 0)
		{
			if (dup2(old_pipe[0], STDIN_FILENO) == -1)
			{
				perror(tmp->argv[0]);
				close(old_pipe[1]);
				free_exit(data, 1);
			}
		}
		close_all_pipes(old_pipe);
	}
}

static void	dup_out(t_cmd *tmp, t_data *data, int new_pipe[2])
{
	if (tmp->outfile >= 0)
	{
		if (dup2(tmp->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			free_exit(data, 1);
		}
		close_all_pipes(new_pipe);
		close(tmp->outfile);
	}
	else if (tmp->next)
	{
		if (new_pipe[1] >= 0)
		{
			if (dup2(new_pipe[1], STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(new_pipe[0]);
				free_exit(data, 1);
			}
		}
		close_all_pipes(new_pipe);
	}
}

void	exec_child(t_cmd *tmp, int new_pipe[2], int old_pipe[2], t_data *data)
{
	check_files(tmp, old_pipe, new_pipe, data);
	dup_in(tmp, data, old_pipe);
	dup_out(tmp, data, new_pipe);
	builtin_in_fork(tmp, data);
	close_fd_new(tmp, tmp->next);
	if (tmp->built_in == -2)
		free_exit(data, 127);
	if (tmp->built_in == -3)
		free_exit(data, 126);
	if (!tmp->argv || !tmp->cmd)
		free_exit(data, 0);
	if (tmp->argv && execve(tmp->argv[0], tmp->argv, data->env_array)
		== -1)
	{
		perror(tmp->cmd);
		free_exit(data, 127);
	}
}

static void	execute_pipeline(t_data *data)
{
	t_cmd	*tmp;
	int		old_pipe[2];
	int		new_pipe[2];

	tmp = data->cmds;
	while (tmp)
	{
		create_pipe(new_pipe, tmp, data);
		tmp->pid = fork();
		if (tmp->pid == -1)
			return (msg_error(data, "fork", 1));
		if (tmp->pid == 0)
			exec_child(tmp, new_pipe, old_pipe, data);
		if (tmp->previous)
			close_all_pipes(old_pipe);
		update_pipe(tmp, old_pipe, new_pipe);
		tmp = tmp->next;
	}
	close_all_pipes(old_pipe);
	wait_for_kids(data->cmds, data);
}

void	execute_command_or_builtin(t_data *data)
{
	t_cmd	*tmp;

	if (!data->cmds)
		return ;
	tmp = data->cmds;
	if (data->cmds->next == NULL)
	{
		if (check_perm(data) == -1)
			return ;
		if (data->cmds->built_in >= 0)
			data->last_exit_code = exec_builtins(tmp, data);
		else
			exec_single_cmd(tmp, data);
	}
	else
		execute_pipeline(data);
}
