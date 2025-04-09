/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:52:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/09 13:02:39 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//gets the current rep from the environment and adds a '>' 
//with minishell, now in color
char	*get_prompt(t_env *env)
{
	char	*str;
	char	*str2;
	int		i;

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
	str = join(str2, "> \e[0;94m\x1b[0m\033[0m");
	free(str2);
	if (!str)
		return (NULL);
	return (str);
}

//does all of the token related tasks and returns error 
//if error there is (-2 syntax error, -1 malloc error, 0 success),
// protected for line == NULL
int	making_tokens(t_token **token, t_env *env, char *lec)
{
	int		check;

	if (!lec)
		return (-1);
	check = parsing_pt1(*token, env, lec);
	free(lec);
	if (check < 0)
		return (check);
	if (check_empty(*token, (*token)->value))
		return (-2);
	check = split_and_sign(token);
	if (check < 0)
		return (check);
	check = assign_types(token);
	if (check < 0)
		return (check);
	if (spec_check(*token) == -2)
		return (-2);
	check = stick_quotes(*token);
	if (check < 0)
		return (check);
	final_types(*token);
	cmd_shuffle(*token);
	return (0);
}

int	data_init(t_data *data, char **envp)
{
	data->env = create_env(envp);
	if (!data->env)
		return (0);
	data->token = NULL;
	data->cmds = NULL;
	data->env_array = NULL;
	data->last_exit_code = 0;
	data->prompt = get_prompt(data->env);
	if (!data->prompt)
		return (free_all_things(data));
	signal(SIGPIPE, &sig_handler_sigpipe);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler);
	data->line = readline(data->prompt);
	if (g_err_code == 130)
	{
		data->last_exit_code = 130;
		g_err_code = 0;
	}
	signal(SIGQUIT, &sig_do_nothing);
	signal(SIGINT, &sig_do_nothing);
	return (1);
}

int	reset_readline(t_data *data)
{
	free(data->prompt);
	data->prompt = get_prompt(data->env);
	if (!data->prompt)
		return (free_all_things(data));
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler);
	data->line = readline(data->prompt);
	if (g_err_code == 130)
	{
		data->last_exit_code = 130;
		g_err_code = 0;
	}
	signal(SIGINT, &sig_do_nothing);
	signal(SIGQUIT, &sig_do_nothing);
	return (1);
}

int	exec_and_co(t_data *data, int check)
{
	data->env_array = env_to_array(data->env);
	if (check != -2 && g_err_code != 130)
		execute_command_or_builtin(data);
	if (check == -2 && !check_empty(data->token, data->token->value))
		data->last_exit_code = 2;
	free_tokens(data->token);
	data->token = NULL;
	g_err_code = 0;
	if (data->env_array)
		free(data->env_array);
	data->env_array = NULL;
	if (data->cmds && update_lst_cmd(data->env, data->cmds) == -1)
		return (free_all_things(data));
	if (data->cmds)
		free_cmds_new(data->cmds, data->cmds->next);
	data->cmds = NULL;
	return (1);
}
