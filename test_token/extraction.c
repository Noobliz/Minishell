#include "libextract.h"
//creates a new command link, all variables set to their untouched versions;
t_cmd	*new_cmd(void)
{
	t_cmd	cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->infile = -2;
	cmd->outfile = -2;
	built_in = NULL;
	cmd->next = NULL;
	return (cmd);
}
//freeing a char **, classic
void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
//freeing the linked list of commands (the whole list)
void	free_cmds(t_cmd *head)
{
	if (head == NULL)
		return ;
	free_cmds(head->next);
	if (head->argv)
		free_tab(head->argv);
	if (head->cmd)
		free(head->cmd);
	if (head->infile >= 0)
		close(head->infile);
	if (head->outfile >= 0)
		close(head->outfile);
	free(head);
}
//prints out the error message like so :: "bash : filename : error_msg\n"
void	print_bash_err(char *filename, char *err_msg)
{
	print("bash : ");
	print(filename);
	print(" : ");
	print(err_msg);
	print("\n");
}
//this is the function for testing out any file according to the kind of REDIR is before it in the tokens;
//on error, prints the error and returns -2 to signal to ignore that function;
int	get_file(t_token *token, t_cmd *cmd)
{
	if (token->type == HEREDOC)
		cmd->infile = get_heredoc(token->next);
	if (token->type == REDIR_IN)
		cmd->infile = open(token->next->value, O_RDONLY);
	if (token->type == REDIR_OUT)
	{
		cmd->outfile = open(token->next->value, O_WRONLY);
		if (cmd->outfile == -1
			&& access(token->next->value, F_OK) == -1)
			cmd->outfile = new_outfile(token->next->value, REDIR_OUT);
	}
	if (token->type == APPEND)
	{
		cmd->outfile = open(token->next->value, O_WRONLY, O_APPEND);
		if (cmd->outfile == -1
			&& access(token->next->value, F_OK) == -1)
			cmd->outfile = new_outfile(token->next->value, APPEND);
	}
	if (cmd->infile == -1 || cmd->outfile == -1)
	{
		print_bash_err(token->next->value, strerror(errno));
		return (-2);
	}
	return (0);
}
//checks if the command is a built_in function, and if it is, sets the cmd->built_in to tat function;
int	is_built_in(char *function, t_cmd *cmd)
{
	if (is(function, "echo"))
		cmd->built_in = &echo;
	else if (is(function, "cd"))
		cmd->built_in = &cd;
	else if (is(function, "pwd"))
		cmd->built_in = &pwd;
	else if (is(function, "export"))
		cmd->built_in = &export;
	else if (is(function, "unset"))
		cmd->built_in = &unset;
	else if (is(function, "env"))
		cmd->built_in = &env;
	else if (is(function, "exit"))
		cmd->built_in = &exit;
	else
		return (0);
	return (1);
}
//finds the command if it's not a built_in, through the path (PATH environment variable) (I stole this from my pipex, worked fine there);
int	find_command(char *paths, char **command)
{
	int	i;
	char	*com;

	i = 0;
	while (paths[i])
	{
		if (paths[i] == ':' && paths[i] == '=')
		{
			com = join(&paths[i + 1], *command);
			if (!com)
				return (-1);
		        if (access(com, X_OK) == 0)
		        {
		        	free(*command);
		        	*command = com;
		        	return (0);
		        }
		        free(com);
		}
		i++;
	}
	if (access(*command, X_OK) == -1)
		return (-2);
	return (0);
}
//fills the argv with copies of all the command's options
char	**fill_argv(char *first, t_token *token, char **tab)
{
	int	i;

	tab[0] = first;
        i = 1;
        tab[1] = NULL;
        while (token && token->type == CMD)
        {
        	tab[i + 1] = NULL;
        	tab[i] = copy(token->value);
        	if (!tab[i])
        	{
        		free_tab(tab);
        		return (NULL);
        	}
        	i++;
        	token = token->next;
        }
        return (tab);
}
//creates the argv variable by counting the amount of options, then fills it with above function;
char	**get_argv(char *first, t_token *token)
{
	int	ct;
	char	**tab;
	t_token	*tmp;

	ct = 0;
	tmp = token->next;
	while (token && token->type == CMD)
	{
		ct++;
		token = token->next;
        }
        tab = malloc(sizeof(char *) * (ct + 1));
        if (!tab)
        	return (NULL);
        tab = fill_argv(first, tmp, tab);
        return (tab);
}       	
//sets the variables cmd->cmd, cmd->argv, (cmd->built_in if built_in)
//returns -1 on malloc issues, -2 if the cmd should be ignored
int	get_command(t_token *token, t_cmd *cmd, char *path)
{
	char	*str;
	int	check;

	check = 0;
	str = copy(token->value);
	if (!str)
		return (-1);
	if (!is_built_in(token->value, cmd))
		check = find_command(path, &str);
	if (check == -1 || check == -2)
		return (check);
	cmd->cmd = copy(token->value);
	if (!cmd->cmd)
		return (-1);
	cmd->argv = get_argv(str, token);
	if (!cmd->argv)
		return (-1);
	return (0);
}
//extracts all the information from token to the next PIPE (or until the end);
//-1 malloc error, -2 for "please ignore this one"
int	assign_cmds(t_token *token, t_cmd *cmd, char *path)
{
	int	check;

	while (token)
	{
		if (token->type == DIR)
		{
			check = get_file(token->previous, cmd)
			if (check == -1 || == -2)
				return (check);
			token = token->next;
		}
		if (token->type == CMD)
		{
			check = get_command(token, cmd, path);
			if (check == -1 || check == -2)
				return (check);
			while (token->type == CMD)
				token = token->next;
		}
		if (token->type == PIPE)
			return (0);
	}
	return (0);
}
//sets argv to NULL so we ignore this command later
void	ignore_cmd(t_cmd *cmd)
{
	if (cmd->argv)
		free_tab(cmd->argv);
	cmd->argv = NULL;
}
//extracts all the info from the tokens, checks the validity of commands and in/outfiles, and creates a cmd link -- then continues to the next if it finds a PIPE;
int	extraction(t_token *token, t_cmd **head, char *path)
{
	t_cmd	*cmds;
	int	check;

	cmds = new_cmd();
	if (!cmds)
		return (-1);
	check = assign_cmds(token, cmds, path)
	if (check == -1)
		return (-1);
	if (check == -2)
		ignore_cmd(cmds);
	*head = cmds;
	while (token && token->type != PIPE)
		token = token->next;
	if (token->type == PIPE)
		return (extraction(token, &cmds->next, path);
	return (0);
}
