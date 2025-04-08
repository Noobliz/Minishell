/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:13:15 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 15:47:00 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//for the variables given as :: ${VAR}
static int	handle_acc_var(t_token *token, t_env *env, int i)
{
	int		j;
	char	*var;

	j = i;
	while (token->value[j] && token->value[j] != '}')
		j++;
	if (!token->value[j])
		return (missing_quote('}'));
	var = get_env(&token->value[i + 2], env);
	if (!var)
		var = replace(token->value, "", i, j + 1);
	else
		var = replace(token->value, var, i, j + 1);
	if (!var)
		return (-1);
	free(token->value);
	token->value = var;
	return (0);
}

//for the variables given as :: $VAR
static int	handle_reg_var(t_token *token, t_env *env, int i)
{
	int		j;
	char	*var;

	j = i + 1;
	while (token->value[j] && is_alphanum(token->value[j]))
		j++;
	if (token->value[i + 1] >= '0' && token->value[i + 1] <= '9')
		j = i + 2;
	var = get_env(&token->value[i + 1], env);
	if (!var)
		var = replace(token->value, "", i, j);
	else
		var = replace(token->value, var, i, j);
	if (!var)
		return (-1);
	free(token->value);
	token->value = var;
	return (0);
}

//for last exit code
static int	handle_lec(t_token *token, char *lec, int i, char a)
{
	char	*str;

	if (a == '\'' || a == '"')
		str = replace(token->value, lec, i, i + 1);
	else
		str = replace(token->value, lec, i, i + 2);
	if (!str)
		return (-1);
	free(token->value);
	token->value = str;
	return (0);
}

static int	handle_var_loop(char *value, int i, int here)
{
	while (value[i] && !(value[i] == '$'
			&& (is_alphanum(value[i + 1])
				|| value[i + 1] == '{' || value[i + 1] == '?'
				|| value[i + 1] == '\'' || value[i + 1] == '\"')))
	{
		if (value[i] == '$' && here > -1)
			return (-2);
		i++;
	}
	return (i);
}

//replaces variables in token->value
//for here == -1, all of them
//for here != -1, the specific one here is on
int	handle_var(t_token *token, t_env *env, int here, char *lec)
{
	int	i;

	i = 0;
	if (here > -1)
		i = here;
	i = handle_var_loop(token->value, i, here);
	if (i == -2)
		return (1);
	if (!token->value[i])
		return (0);
	if (token->value[i + 1] == '{')
		i = handle_acc_var(token, env, i);
	else if (is_alpha(token->value[i + 1]))
		i = handle_reg_var(token, env, i);
	else if (token->value[i + 1] == '?')
		i = handle_lec(token, lec, i, 'o');
	else if (token->value[i + 1] == '\'' || token->value[i + 1] == '"'
		|| (token->value[i +1] <= '9' && token->value[i +1] >= '0'))
		i = handle_lec(token, "", i, token->value[i +1]);
	if (i < 0 || here > -1)
		return (i);
	else
		return (handle_var(token, env, here, lec));
	return (0);
}
