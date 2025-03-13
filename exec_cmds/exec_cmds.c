#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct s_all
{
    char            **cmds;
    char            *cmd;
    int             infile;
    int             outfile;
	//(*f)	built_in;
    struct s_all    *next;
} t_all;

// int	is_builtin(char *cmd)
// {
// 	if (strcmp(cmd, "echo") == 0|| strcmp(cmd, "cd") == 0 || strcmp(cmd, "export") == 0|| strcmp(cmd, "unset") == 0 ||
// 	 strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0)
// 		return (1);
// 	else
// 		return(0);
// }
void    free_tab(char **tab)
{
    int i;
    i = 0;
    while(tab[i])
    {
        free(tab[i]);
        i++;
    }
    if (tab)
        free(tab);
}
void    free_cmds(t_all *head)
{
    if (head == NULL)
        return ;
    free_cmds(head->next);
    if (head->cmds)
        free_tab(head->cmds);
    if (head->cmd)
        free(head->cmd);
    if (head->infile >= 0)
        close(head->infile);
    if (head->outfile >= 0)
        close(head->outfile);
    free(head);
}
int	 count_cmds(t_all *cmds)
{
	t_all *tmp;
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
// trying to do a bit like you :D
void create_close_pipes(int pipes[][2], int cmd_count, t_all *cmds, int create)
{
    int i = 0;
    while (i < cmd_count - 1)
    {
        if (create == 1)
        {
            if (pipe(pipes[i]) == -1)
            {
                perror("pipe");
                //free_cmds(cmds);
                exit(1);
            }
        }
        else if (pipes[i])
            {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
        i++;
    }
}

void execute_pipeline(t_all *cmds, char **envp)
{
    int     cmd_count;
    t_all   *tmp = cmds;
    pid_t pid;
	int	i;

   	cmd_count = count_cmds(tmp);
    if (cmd_count == 0)
        return;
    int pipes[cmd_count - 1][2]; 
    create_close_pipes(pipes, cmd_count, cmds, 1);
    i = 0;
    tmp = cmds;

    while (tmp)
    {
        pid = fork();
        if (pid == -1)
        {
			//free_cmds(tmp);
            perror("fork");
            return;
        }
        if (pid == 0)
        {
            if (tmp->infile == -1)
            {
                printf("infile\n");
                //free_cmds(tmp);
                exit(0);
            }
            if (tmp->infile >= 0)
            {
                dup2(tmp->infile, STDIN_FILENO);
                close(tmp->infile);
            }
           if (tmp->outfile >= 0)
            {
                dup2(tmp->outfile, STDOUT_FILENO);
                close(tmp->outfile);
            }
            // first cmd
            if (i == 0 && cmd_count > 1)
            {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    //free_cmds(tmp);
                    create_close_pipes(pipes, cmd_count, tmp, 0);
                }
            }
            // cmd du milieu
            else if (i > 0 && i < cmd_count -1)
            {
                //if(tmp->infile == 0)
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                //if (tmp->outfile == 0)
                    dup2(pipes[i][1], STDOUT_FILENO);
            }
            // Derniere cmd
            else if (i == cmd_count -1)
                dup2(pipes[i - 1][0], STDIN_FILENO);
            create_close_pipes(pipes, cmd_count, tmp, 0);
            // if (cmds->built_in != NULL)
            // {
            //     cmds->built_in(tmp, envp);
			// 	//free_all
            //     exit(0);
            // }
            // cmd externe
            if (tmp->cmds && (tmp->outfile != -1) && execve(tmp->cmds[0], tmp->cmds, envp) == -1)
            {
                perror(tmp->cmd);
			    //free_cmds(tmp);
                exit(127);
            }
            //free_cmds(tmp); ??
        }
        tmp = tmp->next;
        i++;
    }
    create_close_pipes(pipes, cmd_count, cmds, 0);
    i = 0;
    while (i < cmd_count)
    {
        wait(NULL);
        i++;
    }
	//free_cmds(cmds);
}

void execute_command_or_builtin(t_all *cmds, char **envp)
{
    if (!cmds)
        return;

    //if (cmds->next == NULL && cmds->built_in != NULL)
        //cmds->built_in(cmds, envp);
   // else
        execute_pipeline(cmds, envp);
}


// Fonction utilitaire pour créer une commande juste pour le test
// t_all *create_cmd(char *path, char **args, char *infile, char *outfile)
// {
//     t_all *cmd = malloc(sizeof(t_all));
//     if (!cmd)
//         return NULL;
    
//     cmd->cmds = args;
//     cmd->cmd = path;
//     cmd->infile = 0;
//     cmd->outfile = 0;
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

//     char *args1[] = {"/bin/cat", NULL};
//     t_all *cmd1 = create_cmd("/bin/cat", args1, "infile.txt", NULL);
//     char *args2[] = {"/usr/bin/grep", "mini", NULL};
//     t_all *cmd2 = create_cmd("/usr/bin/grep", args2, NULL, NULL);
//     char *args3[] = {"/usr/bin/wc", "-l", NULL};
//     t_all *cmd3 = create_cmd("/usr/bin/wc", args3, NULL, "outfile.txt");
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
