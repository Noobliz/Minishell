#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "test_token/libextract.h"
#include <readline/readline.h>
#include <readline/history.h>

void  free_tokens(t_token *token)
{
  if (!token)
    return ;
  free_tokens(token->next);
  if (token->value)
  	free(token->value);
  free(token);
}

void	disp_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->var);
		env = env->next;
	}
}

void print_tokens(t_token *tokens)
{
    printf("\n");
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

void print_cmds(t_cmd *cmd)
{
  int i;
  while (cmd)
  {
    print("\n");
    printf("cmd :: %s\noptions :: \n", cmd->cmd);
    i = 0;
    while (cmd->argv && cmd->argv[i])
    {
      printf("%s, ", cmd->argv[i]);
      i++;
    }
    printf("\ninfile : %d, outfile %d\n", cmd->infile, cmd->outfile);
    if (cmd->built_in)
      printf("is builtin\n");
    else
      printf("is not builtin\n");
    cmd = cmd->next;
  }
}

int	 count_cmds(t_cmd *cmds)
{
	t_cmd *tmp;
	tmp = cmds;
	int count;

	count = 0;
	while(tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return(count);
}

void    wait_for_kids(t_cmd *cmds)
{
    t_cmd *tmp;
    tmp = cmds;
    int status;
     while (tmp)
    {
        waitpid(tmp->pid, &status, 0);
        tmp = tmp->next;
    }
}
void    check_files(t_cmd *current)
{
    if (current->infile == -1)
    {
        //free_cmds
        exit(1);
    }
    if (current->outfile == -1)
    {
        //free_cmds;
        exit(1);
    }
}

void    update_pipe(t_cmd *current, int old_pipe[2], int new_pipe[2])
{
    if (current->next)
        {
            old_pipe[0] = new_pipe[0];
            old_pipe[1] = new_pipe[1];
        }
}

void    dup_and_close(t_cmd *tmp, int old_pipe[2], int new_pipe[2], int i)
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
void execute_pipeline(t_cmd *cmds, char **envp)
{
    t_cmd *tmp = cmds;
    pid_t pid;
    int i;
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
                //free_cmds
                return;
            }
        }
        tmp->pid = fork();
        if (tmp->pid == -1)
        {
            perror("fork");
            return;
        }
        if (tmp->pid == 0)
        {
            check_files(tmp);
            dup_and_close(tmp, old_pipe, new_pipe, i);
            //if (tmp->is_builtin)
            if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
            {
                perror(tmp->cmd);
                //free_cmds
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

void execute_command_or_builtin(t_cmd *cmds, char **envp)
{
    if (!cmds)
        return;
    t_cmd   *tmp;
    tmp = cmds;
    if (cmds->next == NULL /*&& cmds->built_in == NULL*/)
    {
        printf("coucou\n");
        if(tmp->infile >= 0)
        {
            dup2(tmp->infile, STDIN_FILENO);
        }
        if(tmp->outfile >= 0)
        {
            dup2(tmp->outfile, STDOUT_FILENO);
        }
        if (tmp->argv && execve(tmp->argv[0], tmp->argv, envp) == -1)
        {
            perror(tmp->cmd);
            exit(127);
        }
        return ;
    }
    //else if (cmds->next == NULL && cmds->built_in != NULL)
        //cmds->built_in(cmds, envp);
    else
        execute_pipeline(cmds, envp);
}
int main(int argc, char **argv, char **envp)
{
  (void)argc;
  if(argv)
  {
	t_env	*n_env = env_item("PATH=here", 0);
	n_env->next = env_item("NUM=herealso", 0);
	int i = 0;
	t_token *token = NULL;
	//char  *str = copy("cat | <example ls -l | wc -l >output.txt");
	char *path = "/nfs/homes/lguiet/.nix-profile/bin:/nix/var/nix/profiles/default/bin:/nfs/homes/lguiet/bin:/nfs/homes/lguiet/.nix-profile/bin:/nix/var/nix/profiles/default/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin";
  char *str;
    t_cmd *head = NULL;
  while(1)
  {
    str = readline("minishell> ");
	  if (!str)
	    return (0);
    if (*str)
      add_history(str);
	token = new_token(copy(str), CMD, NULL);
	  if (!token)
	    return (0);
    i = parsing_pt_2(token, n_env);
	  if (i == -1)
	    printf("malloc error\n");
	  if (i == -2)
	    printf("only one quote\n");
	  i = trim_split_tokens(token);
	  if (i == -1)
	    printf("malloc error\n");
	//print_tokens(token);
      spec_check(token);
    if (assign_types(&token) == -1 || !token)
        printf("malloc error\n");
    cmd_shuffle(token);
    extraction(token, &head, path);
    print_cmds(head);
    //execute_pipeline(head, envp);
    execute_command_or_builtin(head, envp);
    free(str);
    free_tokens(token);
    free_cmds_bw(head);
    //head = NULL;
  }
  free(n_env->next);
  free(n_env);
  }
  //print_tokens(token);
	return (0);
}





// Fonction utilitaire pour créer une commande juste pour le test
// t_cmd *create_cmd(char *path, char **args, char *infile, char *outfile)
// {
//     t_cmd *cmd = malloc(sizeof(t_cmd));
//     if (!cmd)
//         return NULL;
    
//     cmd->cmds = args;
//     cmd->cmd = path;
//     cmd->infile = -2;
//     cmd->outfile = -2;
//     if(infile)
//         cmd->infile = open(infile, O_RDONLY);
//     if (infile && cmd->infile == -1)
//         perror("Error opening input file");

//     if (outfile)
//         cmd->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (outfile && cmd->outfile == -1)
//         perror("Error opening output file");
	

//     cmd->next = NULL;
//     return cmd;
// }

// int main(int argc, char **argv, char **envp)
// {
//     (void)argc;
//     (void)argv;

//     char *args1[] = {"/bin/lsssssssss", NULL};
//     t_cmd *cmd1 = create_cmd("/bin/ls", args1, NULL, "infile.txt");
//     char *args2[] = {"/usr/bin/grep", "outfile.txt", NULL};
//     t_cmd *cmd2 = create_cmd("/usr/bin/grep", args2, NULL, NULL);
//     char *args3[] = {"/usr/bin/touch", "MDR", NULL};
//     t_cmd *cmd3 = create_cmd("/usr/bin/touch", args3, NULL, NULL);
//     cmd1->next = cmd2;
//     cmd2->next = cmd3;
//     // while(cmd1)
//     // {
//     //     printf("args = %s\n", cmd1->cmd);
//     //     cmd1 = cmd1->next;
//     // }
//     execute_pipeline(cmd1, envp);
//     free(cmd1);
//     free(cmd2);
//     free(cmd3);

//     return 0;
// }
