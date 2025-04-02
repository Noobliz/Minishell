#include "libbig.h"

int g_err_code;

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
// int	free_all_things(t_env *env, t_token *token, t_cmd *cmd, char *prompt)
// {
// 	if (env)
// 		free_env(env);
// 	if (token)
// 		free_tokens(token);
// 	if (cmd)
// 		free_cmds_new(cmd, cmd->next);
// 	if (prompt)
// 		free(prompt);
// 	rl_clear_history();
// 	return (0);
// }
int	free_all_things(t_data *data)
{
	if (data->env_array)
	{
		free(data->env_array);
		data->env_array = NULL;
	}
	if (data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	if (data->token)
	{
		free_tokens(data->token);
		data->token = NULL;
	}
	if (data->cmds)
	{
		free_cmds_new(data->cmds, data->cmds->next);
		data->cmds = NULL;
	}
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	rl_clear_history();
	return (0);
}

//gets the current path from the environment and adds a '>' for clarity, can change this anytime
char	*get_prompt(t_env *env)
{
	char	*str;
	char	*str2;
	int	i;

	str = get_env("PWD", env);
	i = 0;
	if (!str)
		str = getcwd(NULL, 0);
	while (str[i])
		i++;
	while (i && str[i - 1] != '/')
		i--;
	str2 = join("\e[0;94mMinishell -- ", &str[i]);
	if (!str2)
		return (NULL);
	if (get_env("PWD", env) == NULL)
		free(str);
	str = join(str2, ">  \e[0;94m\x1b[0m\033[0m");
	free(str2);
	if (!str)
		return (NULL);
	return (str);
}

//hasty itoa (for the err_code)
char	*num_str(int code)
{
	char	*lec;
	int	len;
	int	tens;

	len = 0;
	tens = 1;
	while (code / tens > 0)
	{
		len++;
		tens = tens * 10;
	}
	if (code == 0)
		len = 1;
	lec = malloc(len + 1);
	if (!lec)
		return (NULL);
	lec[len] = '\0';
	while (len > 0)
	{
		lec[len -1] = code % 10 + '0';
		len--;
		code = code / 10;
	}
	return (lec);
}

//does all of the token related tasks and returns error if error there is (-2 syntax error, -1 malloc error, 0 success), protected for line == NULL
int	making_tokens(t_token **token, t_env *env, int code)
{
	int	check;
	char	*lec;

	if (!(*token)->value || !(*token)->value[0])
		return (-2);
	lec = num_str(code);
	if (!lec)
		return (-1);
	check = parsing_pt1(*token, env, lec);
	free(lec);
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


void	sig_handler(int code)
{
	(void)code;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

	g_err_code = 130;
}

void	sig_handler_heredoc(int code)
{
	(void)code;
	write(1, "\n", 1);
	g_err_code = 130;
	close(0);
}
void	sig_handler_sigpipe(int code)
{
	(void)code;
	g_err_code = 13;
}

void	sig_do_nothing(int code)
{
	(void)code;
	write(1, "\n", 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		check;

	ft_bzero(&data, sizeof(t_data));
	if (argc > 1 || argv[1])
		return (0);

	data.env = create_env(envp);
	if (!data.env)
		return (0);

	data.prompt = get_prompt(data.env);
	if (!data.prompt)
		return (free_all_things(&data));
	signal(SIGPIPE, &sig_handler_sigpipe);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler);
	data.line = readline(data.prompt);
	data.env_array = NULL;
	if (g_err_code == 130)
	{
		data.last_exit_code = 130;
		g_err_code = 0;
	}
	signal(SIGQUIT, &sig_do_nothing);
	signal(SIGINT, &sig_do_nothing);

	while (data.line)
	{
		add_history(data.line);

		data.token = new_token(data.line, CMD, NULL);
		if (!data.token)
			return (free_all_things(&data));

		check = making_tokens(&data.token, data.env, data.last_exit_code);
		if (check == -1)
			return (free_all_things(&data));

		if (check != -2 && extraction(data.token, &data.cmds, get_env("PATH", data.env), &data) < 0)
			return (free_all_things(&data));

		add_count_cmds(data.cmds);
		free_tokens(data.token);
		data.token = NULL;

		data.env_array = env_to_array(data.env);

		if (check != -2 && g_err_code != 130)
		{
			execute_command_or_builtin(&data);
		}
		if (g_err_code == 13)
			free_all_things(&data);
		g_err_code = 0;
		free(data.env_array);
		data.env_array = NULL;
		if (update_lst_cmd(data.env, data.cmds) == -1)
			return (free_all_things(&data));
		if (data.cmds)
			free_cmds_new(data.cmds, data.cmds->next);
		data.cmds = NULL;

		free(data.prompt);
		data.prompt = get_prompt(data.env);
		if (!data.prompt)
			return (free_all_things(&data));

		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, &sig_handler);
		data.line = readline(data.prompt);
		if (g_err_code == 130)
		{
			data.last_exit_code = 130;
			g_err_code = 0;
		}
		signal(SIGINT, &sig_do_nothing);
		signal(SIGQUIT, &sig_do_nothing);
	}
	//free(data.line);
	return (free_all_things(&data));
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*env = NULL;
// 	t_token	*token = NULL;
// 	t_cmd	*cmd = NULL;
// 	char	*line;
// 	char	*prompt;
// 	int	check;
// 	char	**env_array;

// 	g_err_code = 0;
// 	//no arguments allowed
// 	if (argc > 1 || argv[1])
// 		return (0);
// 	//create the environment
// 	env = create_env(envp);
// 	if (!env)
// 		return (0);
// 	//getting prompt for readline (here PWD env variable)
// 	prompt = get_prompt(env);
// 	if (!prompt)
// 		return (free_all_things(env, token, cmd, NULL));
// 	signal(SIGQUIT, SIG_IGN);
// 	signal(SIGINT, &sig_handler);
// 	line = readline(prompt);
// 	if (g_err_code == 130)
// 	{
// 		// last_exit_code = g_err_code; --> l'idee c'est de mettre a jour pour $?
// 		g_err_code = 0;
// 	}
// 	signal(SIGQUIT, &sig_do_nothing);
// 	signal(SIGINT, &sig_do_nothing);
// 	// if !line??? > pour ctrl D
// 	while (line)
// 	{
// 		add_history(line);
// 		//creates first token based on the line read
// 		token = new_token(line, CMD, NULL);
// 		if (!token)
// 		{
// 			free(line);
// 			return (free_all_things(env, token, cmd, prompt));
// 		}
// 		//goes into the parsing for tokens, assigning types etc.
// 		check = making_tokens(&token, env);
// 		if (check == -1)
// 			return (free_all_things(env, token, cmd, prompt));
// 		//print_tokens(token); //print tokens here to check in cas something goes wrong
// 		//extracting info only if there were no syntax errors during tokenization
// 		if (check != -2 && extraction(token, &cmd, get_env("PATH", env), env) < 0)
// 			return (free_all_things(env, token, cmd, prompt));
// 		//adding total number of cmds to every cmd (can delete this if unnecessary)
// 		add_count_cmds(cmd);
// 		//freeing tokens since they're no longer necessary
// 		free_tokens(token);
// 		token = NULL; //!!super important to reset the variables to avoid segfault
// 		//print_cmds(cmd); // printing for error-tracing again
// 		//exec here, you can replace with your own
// 		env_array = env_to_array(env);
// 		if (g_err_code != 130)
// 		{
// 			if (cmd && (cmd->argv || cmd->next))
// 				execute_command_or_builtin(cmd, env, env_array);
// 		}
// 		else
// 			g_err_code = 0;	
// 			//reinit g_err_code a chaque fois pcq sinon y a plus rien qui marche
			
// 		//if (tmp_exec(cmd, env) == -1)
// 			//return (free_all_things(env, token, cmd, prompt));
// 		//same free/assign NULL combo for cmds now that we're not using them anymore
// 		free(env_array);
// 		env_array = NULL;
// 		free_cmds(cmd);
// 		cmd = NULL;
// 		//getting next line -- we don't free line because free_tokens already does it
// 		signal(SIGQUIT, SIG_IGN);
// 		signal(SIGINT, &sig_handler);
// 		line = readline(prompt);
// 		if (g_err_code == 130)
// 		{
// 			// last_exit_code = g_err_code; --> l'idee c'est de mettre a jour pour $?
// 			g_err_code = 0;
// 		}
// 		signal(SIGINT, &sig_do_nothing);
// 		signal(SIGQUIT, &sig_do_nothing);
// 	}
// 	//free last line which hasn't gone into tokens, then free everything !
// 	free(line);
// 	return (free_all_things(env, token, cmd, prompt));
// }

