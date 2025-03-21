#include "libbig.h"

//used to be is() function, but it existed somewhere else, didn't want to go looking for it
//checks if the str cat and copy are the same
int isis(char *cat, char *copy)
{
  int i = 0;
  
  if (!cat || !copy)
    return (0);
  while (cat[i] && cat[i] == copy[i])
    i++;
  if (cat[i] != copy[i])
    return (0);
  return (1);
}
//prints tokens
void print_tokens(t_token *tokens)
{
    printf("\n");
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}
//prints cmds
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
    if (cmd->built_in != -1)
      printf("is builtin\n");
    else
      printf("is not builtin\n");
    cmd = cmd->next;
  }
}
//frees every single possible thing :] including the prompt (for me, because I malloc it, but you can easily get rid of that or send it NULL instead)
int	free_all_things(t_env *env, t_token *token, t_cmd *cmd, char *prompt)
{
	if (env)
		free_env(env);
	if (token)
		free_tokens(token);
	if (cmd)
		free_cmds_new(cmd, cmd->next);
	if (prompt)
		free(prompt);
	rl_clear_history();
	return (0);
}
//gets the current path from the environment and adds a '>' for clarity, can change this anytime
char	*get_prompt(t_env *env)
{
	char	*str;

	str = get_env("PWD", env);
	str = join(str, "> ");
	if (!str)
		return (NULL);
	return (str);
}
//does all of the token related tasks and returns error if error there is (-2 syntax error, -1 malloc error, 0 success), protected for line == NULL
int	making_tokens(t_token **token, t_env *env)
{
	int	check;

	if (!(*token)->value)
		return (-2);
	check = parsing_pt_2(*token, env);
	if (check < 0)
		return (check);
	check = fix_quotes(*token);
	if (check < 0)
		return (check);
	all_cmd_type(*token);
	check = spec_check(*token);
	if (check < 0)
		return (check);
	check = assign_types(token);
	if (check < 0)
		return (check);
	cmd_shuffle(*token);
	return (0);
}
//my exec, change this to what you're doing, mine just calls my built_ins with no fork :]
//feel free to steal my infile/outfile workaround, I stole it myself like a true dev from some guy online
int	tmp_exec(t_cmd *cmd, t_env *env)
{
	int	infile;
	int	outfile;

	while (cmd)
	{
		if (cmd->infile > 0)
		{
			infile = dup(0);
			if (infile == -1)
				return (-1);
			close(0);
			if (dup2(cmd->infile, 0) == -1)
				return (-1);
		}
		if (cmd->outfile > 0)
		{
			outfile = dup(1);
			if (outfile == -1)
				return (-1);
			close(1);
			if (dup2(cmd->outfile, 1) == -1)
				return (-1);
		}
		if (built_in_att1(cmd->built_in, cmd->argv, NULL, env) == -1)
			return (-1);
		if (cmd->infile > 0)
		{
			close(0);
			if (dup2(infile, 0) == -1)
				return (-1);
			close(infile);
		}
		if (cmd->outfile > 0)
		{
			close(1);
			if (dup2(outfile, 1) == -1)
				return (-1);
			close(outfile);
		}
		cmd = cmd->next;
	}
	return (0);
}

// transform the t_env to char ** for execve
int	env_size(t_env *env)
{
	int	size = 0;

	while (env)
	{
		if (env->var)
			size++;
		env = env->next;
	}
	return (size);
}

char	**env_to_array(t_env *env)
{
	int		i = 0;
	char	**env_array;
	int		size;
	
	size = env_size(env);
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (perror("malloc"), NULL);
	while (env)
	{
		if (env->var)
		{
			env_array[i] = env->var;
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
//and my main !! loops through readline with PWD prompt, until you send exit
int	main(int argc, char **argv, char **envp)
{
	t_env	*env = NULL;
	t_token	*token = NULL;
	t_cmd	*cmd = NULL;
	char	*line;
	char	*prompt;
	int	check;

	//no arguments allowed
	if (argc > 1 || argv[1])
		return (0);
	//create the environment
	env = create_env(envp);
	if (!env)
		return (0);
	//getting prompt for readline (here PWD env variable)
	prompt = get_prompt(env);
	if (!prompt)
		return (free_all_things(env, token, cmd, NULL));
	line = readline(prompt);
	while (!isis(line, "exit"))
	{
		add_history(line);
		//creates first token based on the line read
		token = new_token(line, CMD, NULL);
		if (!token)
		{
			free(line);
			return (free_all_things(env, token, cmd, prompt));
		}
		//goes into the parsing for tokens, assigning types etc.
		check = making_tokens(&token, env);
		if (check == -1)
			return (free_all_things(env, token, cmd, prompt));
		//print_tokens(token); //print tokens here to check in cas something goes wrong
		//extracting info only if there were no syntax errors during tokenization
		if (check != -2 && extraction(token, &cmd, get_env("PATH", env)) < 0)
			return (free_all_things(env, token, cmd, prompt));
		//adding total number of cmds to every cmd (can delete this if unnecessary)
		add_count_cmds(cmd);
		//freeing tokens since they're no longer necessary
		free_tokens(token);
		token = NULL; //!!super important to reset the variables to avoid segfault
		//print_cmds(cmd); // printing for error-tracing again
		//exec here, you can replace with your own
		execute_command_or_builtin(cmd, env, env_to_array(env));
		if (tmp_exec(cmd, env) == -1)
			return (free_all_things(env, token, cmd, prompt));
		//same free/assign NULL combo for cmds now that we're not using them anymore
		free_cmds(cmd);
		cmd = NULL;
		//getting next line -- we don't free line because free_tokens already does it
		line = readline(prompt);
	}
	//free last line which hasn't gone into tokens, then free everything !
	free(line);
	return (free_all_things(env, token, cmd, prompt));
}
