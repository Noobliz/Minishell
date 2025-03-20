
#include "libbig.h"

void	wait_for_kids(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		status;

	tmp = cmds;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		tmp = tmp->next;
	}
}
void	check_files(t_cmd *current)
{
	if (current->infile == -1)
	{
		// free_cmds
		exit(1);
	}
	if (current->outfile == -1)
	{
		// free_cmds;
		exit(1);
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

void	dup_and_close(t_cmd *tmp, int old_pipe[2], int new_pipe[2], int i)
{
	if (tmp->infile >= 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		close(tmp->infile);
	}
	else if (i > 0)
	{
		dup2(old_pipe[0], STDIN_FILENO);
		close(old_pipe[0]);
		close(old_pipe[1]);
	}
	if (tmp->outfile >= 0)
	{
		dup2(tmp->outfile, STDOUT_FILENO);
		close(tmp->outfile);
	}
	else if (tmp->next)
	{
		dup2(new_pipe[1], STDOUT_FILENO);
		close(new_pipe[0]);
		close(new_pipe[1]);
	}
}
void	execute_pipeline(t_cmd *cmds, t_env *env, char **envp)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmds;
	int old_pipe[2]; // Stocke le pipe précédent
	int new_pipe[2]; // Stocke le pipe actuel
	i = 0;
	while (tmp)
	{
		if (tmp->next)
		{
			if (pipe(new_pipe) == -1)
			{
				perror("pipe");
				// free_cmds
				return ;
			}
		}
		tmp->pid = fork();
		if (tmp->pid == -1)
		{
			perror("fork");
			return ;
		}
		if (tmp->pid == 0)
		{
			check_files(tmp);
			dup_and_close(tmp, old_pipe, new_pipe, i);
			if (tmp->built_in >= 0)
			{
				if(built_in_att1(tmp->built_in, tmp->argv, envp, env))
				{
					//free_all
					exit(0);
				}
				else
				{
					//free_all
					exit(1);
				}
			}
			if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
			{
				perror(tmp->cmd);
				// free_cmds
				exit(127);
			}
		}
		if (i > 0)
		{
			close(old_pipe[0]);
			close(old_pipe[1]);
		}
		update_pipe(tmp, old_pipe, new_pipe);
		tmp = tmp->next;
		i++;
	}
	if (cmds->next)
	{
		close(old_pipe[0]);
		close(old_pipe[1]);
	}
	wait_for_kids(cmds);
}

void	execute_command_or_builtin(t_cmd *cmds, t_env *env, char **envp)
{
	t_cmd	*tmp;
	int		status;

	if (!cmds)
		return ;
	tmp = cmds;
	// only one cmd and not a builtin
	if (cmds->next == NULL && cmds->built_in < 0)
	{
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			if (tmp->infile >= 0)
			{
				dup2(tmp->infile, STDIN_FILENO);
			}
			if (tmp->outfile >= 0)
			{
				dup2(tmp->outfile, STDOUT_FILENO);
			}
			if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
			{
				perror(tmp->cmd);
				exit(127);
			}
		}
		waitpid(tmp->pid, &status, 0);
		return ;
	}
	// if builtin alone
	else if (cmds->next == NULL && cmds->built_in >= 0)
		built_in_att1(cmds->built_in, cmds->argv, envp, env);
	// if pipeline
	else
		execute_pipeline(cmds, env, envp);
}
