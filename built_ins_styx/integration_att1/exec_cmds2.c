
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
		// free_all
		exit(1);
	}
	if (current->outfile == -1)
	{
		// free_all
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
	if (tmp->infile >= 0 && tmp->built_in < 0) // there is something i don't understand here about builtins
	{
		if (dup2(tmp->infile, STDIN_FILENO) == -1)
			perror("dup2");
		close(tmp->infile);
	}
	else if (i > 0)
	{
		if (dup2(old_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(old_pipe[0]);
		close(old_pipe[1]);
	}
	if (tmp->outfile >= 0 && tmp->built_in < 0)
	{
		if (dup2(tmp->outfile, STDOUT_FILENO) == -1)
			perror("dup2");
		close(tmp->outfile);
	}
	else if (tmp->next)
	{
		if (dup2(new_pipe[1], STDOUT_FILENO) == -1)
			perror("dup2");
		close(new_pipe[0]);
		close(new_pipe[1]);
	}
}
int	exec_builtins(t_cmd *tmp, char **envp, t_env *env)
{
	int	infile;
	int	outfile;
	if (tmp->infile >= 0)
	{
		printf("infile");
		infile = dup(0); //sauvegarde de stdin
		if (infile == -1)
			return (-1);
		close(0);
		if (dup2(tmp->infile, STDIN_FILENO) == -1)
			return (-1);
		close(tmp->infile);
		//tmp->infile = -2; // need to discuss this
	}
	if (tmp->outfile >= 0)
	{
		printf("outfile");
		outfile = dup(1);
		if (outfile == -1)
			return (-1);
		close(1);
		if (dup2(tmp->outfile, STDOUT_FILENO) == -1)
			return (-1);
		close(tmp->outfile);
	}
	if (built_in_att1(tmp->built_in, tmp->argv, envp, env) == -1)
		return (-1);
	if (tmp->infile >= 0)
	{
		close(0);
		if (dup2(infile, 0) == -1) // restauration de stdin
			perror("dup2");
		close(infile);
	}
	if (tmp->outfile >= 0)
	{
		close(1);
		if (dup2(outfile, 1) == -1)
			perror("dup2") ;
		close(outfile);
	}
	return (0);
}

void	execute_pipeline(t_cmd *cmds, t_env *env, char **envp)
{
	t_cmd	*tmp;
	int		i;
	int old_pipe[2]; // Stocke le pipe précédent
	int new_pipe[2]; // Stocke le pipe actuel

	tmp = cmds;
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
				if (exec_builtins(tmp, envp, env) == -1)
				{
					//free_all
					exit(1);
				}
				else
				{
					//free_all
					exit(0);
				}
			}
			else if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
			{
				perror(tmp->cmd);
				// free_all
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
	// only one cmd
	if (cmds->next == NULL)
	{
		if (cmds->built_in >= 0)
		{
			exec_builtins(tmp, envp, env);
			return ;
		}
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			if (tmp->infile >= 0)
			{
				if(dup2(tmp->infile, STDIN_FILENO) == -1)
				{
					perror("dup2");
					//free_all
				}
			}
			if (tmp->outfile >= 0)
			{
				if(dup2(tmp->outfile, STDOUT_FILENO) == -1)
				{
					perror("dup2");
					//free_all
				}
			}
			if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
			{
				perror(tmp->cmd);
				//free_all
				exit(127);
			}
		}
		waitpid(tmp->pid, &status, 0);
		return ;
	}
	// if pipeline
	else
		execute_pipeline(cmds, env, envp);
}
