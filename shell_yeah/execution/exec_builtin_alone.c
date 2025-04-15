/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_alone.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:22:30 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/10 17:27:49 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

int	check_perm(t_data *data)
{
	if (data->cmds->built_in == -2)
	{
		if (data->cmds->infile == -1 || data->cmds->outfile == -1)
			data->last_exit_code = 1;
		else
			data->last_exit_code = 127;
		return (-1);
	}
	if (data->cmds->built_in == -3)
	{
		if (data->cmds->infile == -1 || data->cmds->outfile == -1)
			data->last_exit_code = 1;
		else
			data->last_exit_code = 126;
		return (-1);
	}
	if (!data->cmds->cmd || !data->cmds->argv)
	{
			data->last_exit_code = 0;
			return (-1);
	}
	return (0);
}

static int	restore_in_out(t_cmd *tmp, int infile, int outfile)
{
	if (tmp->infile >= 0)
	{
		close(0);
		if (dup2(infile, 0) == -1)
		{
			perror("dup2");
			return (1);
		}
		close(infile);
	}
	if (tmp->outfile >= 0)
	{
		close(1);
		if (dup2(outfile, 1) == -1)
		{
			perror("dup2");
			return (1);
		}
		close(outfile);
	}
	return (0);
}

static int	dup_infile(t_cmd *tmp, int *infile)
{
	*infile = dup(0);
	if (*infile == -1)
		return (1);
	close(0);
	if (dup2(tmp->infile, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (1);
	}
	return (0);
}

static int	dup_outfile(t_cmd *tmp, int *outfile)
{
	*outfile = dup(1);
	if (*outfile == -1)
		return (1);
	close(1);
	if (dup2(tmp->outfile, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		return (1);
	}
	return (0);
}

int	exec_builtins(t_cmd *tmp, t_data *data)
{
	int	infile;
	int	outfile;
	int	exit_code;

	outfile = 0;
	infile = 0;
	if (tmp->infile >= 0)
	{
		if (dup_infile(tmp, &infile) == -1)
			return (1);
	}
	if (tmp->outfile >= 0)
	{
		if (dup_outfile(tmp, &outfile) == -1)
			return (1);
	}
	exit_code = built_in(tmp->built_in, tmp->argv, data, 0);
	if (restore_in_out(tmp, infile, outfile) == 1)
		return (1);
	return (exit_code);
}
