/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_free_close_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:39:01 by lguiet            #+#    #+#             */
/*   Updated: 2025/03/28 15:45:48 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

void	close_all_pipes(int pipe[2])
{
	close(pipe[0]);
	close(pipe[1]);
}

void	free_exit(t_data *data, int exit_code)
{
	free_all_things(data);
	exit(exit_code);
}

static void	close_fd(t_cmd *head)
{
	if (head == NULL)
		return ;
	close_fd(head->next);
	if (head->infile > 0)
	{
		close(head->infile);
		head->infile = -2;
	}
	if (head->outfile > 0)
	{
		close(head->outfile);
		head->outfile = -2;
	}
}

static void	close_fd_bw(t_cmd *head)
{
	if (head == NULL)
		return ;
	close_fd_bw(head->previous);
	if (head->infile > 0)
	{
		close(head->infile);
		head->infile = -2;
	}
	if (head->outfile > 0)
	{
		close(head->outfile);
		head->outfile = -2;
	}
}
void	close_fd_new(t_cmd *prev, t_cmd *next)
{
	if (!prev && !next)
		return ;
	close_fd(next);
	close_fd_bw(prev);
}