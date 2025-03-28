/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:40:39 by lguiet            #+#    #+#             */
/*   Updated: 2025/03/28 17:08:36 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbig.h"

void	msg_error(t_data *data, char *str, int code)
{
	perror(str);
	data->last_exit_code = code;
}

void	wait_for_kids(t_cmd *cmds)
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
				cmds->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				cmds->last_exit_code = 128 + WTERMSIG(status);
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

void	update_pipe(t_cmd *current, int old_pipe[2], int new_pipe[2])
{
	if (current->next)
	{
		old_pipe[0] = new_pipe[0];
		old_pipe[1] = new_pipe[1];
	}
}

void	dup_in(t_cmd *tmp, t_data *data, int old_pipe[2])
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

void	dup_out(t_cmd *tmp, t_data *data, int new_pipe[2])
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

void	dup_and_close(t_cmd *tmp, int old_pipe[2], int new_pipe[2],
		t_data *data)
{
	dup_in(tmp, data, old_pipe);
	dup_out(tmp, data, new_pipe);
}

void	builtin_in_fork(t_cmd *tmp, t_data *data)
{
	if (tmp->built_in >= 0)
	{
		if (built_in_att1(tmp->built_in, tmp->argv, data->env_array, data) ==
			-1)
		{
			close_fd_new(tmp, tmp->next);
			free_exit(data, 1);
		}
		close_fd_new(tmp, tmp->next);
		free_exit(data, 0);
	}
}

void	execute_pipeline(t_data *data)
{
	t_cmd	*tmp;
	int		i;
	int		old_pipe[2];
	int		new_pipe[2];

	tmp = data->cmds;
	i = 0;
	while (tmp)
	{
		if (tmp->next)
		{
			if (pipe(new_pipe) == -1)
				return (msg_error(data, "pipe", -1));
		}
		tmp->pid = fork();
		if (tmp->pid == -1)
			return (msg_error(data, "fork", -1));
		if (tmp->pid == 0)
		{
			check_files(tmp, old_pipe, new_pipe, data);
			dup_and_close(tmp, old_pipe, new_pipe, data);
			builtin_in_fork(tmp, data);
			close_fd_new(tmp, tmp->next);
			if (!tmp->argv)
				free_exit(data, 127);
			if (tmp->argv && execve(tmp->argv[0], tmp->argv, data->env_array) ==
				-1)
			{
				perror(tmp->cmd);
				free_exit(data, 127);
			}
		}
		if (i > 0)
			close_all_pipes(old_pipe);
		update_pipe(tmp, old_pipe, new_pipe);
		tmp = tmp->next;
		i++;
	}
	close_all_pipes(old_pipe);
	wait_for_kids(data->cmds);
}

void	dup_files(t_cmd *tmp, t_data *data)
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
			wait_for_kids(tmp);
		}
	}
	else
		execute_pipeline(data);
}
