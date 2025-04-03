/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:13:15 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/03 12:55:54 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	replace_pt2(char *newstr, char *s, char *var, int where)
{
	int	i;
	int	len;

	i = 0;
	while (i < where)
	{
		newstr[i] = s[i];
		i++;
	}
	len = 0;
	while (var[len])
	{
		newstr[i + len] = var[len];
		len++;
	}
	len += i;
	return (len);
}

//ok so this one is supposed to take in
//the string (token->value)
//the variable (from get_env)
//the index of the $
//returns that same string with the variable substituted
//(ie for VAR=VALUE we go from "here $VAR" to "here VALUE")
//!!it does not free the inital strings given, for malloc protection reasons
char	*replace(char *s, char *var, int where, int next)
{
	int		len;
	char	*newstr;

	len = 0;
	while (s[next + len])
		len++;
	len = len + where + len_str(var);
	newstr = malloc(len + 1);
	if (!newstr)
		return (NULL);
	len = replace_pt2(newstr, s, var, where);
	while (s[next])
	{
		newstr[len] = s[next];
		len++;
		next++;
	}
	newstr[len] = '\0';
	return (newstr);
}

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

//for the variables given like this :: $VAR
static int	handle_reg_var(t_token *token, t_env *env, int i)
{
	int		j;
	char	*var;

	j = i + 1;
	while (token->value[j] && is_alphanum(token->value[j]))
		j++;
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

static int	handle_lec(t_token *token, char *lec, int i)
{
	char	*str;

	str = replace(token->value, lec, i, i + 2);
	if (!str)
		return (-1);
	free(token->value);
	token->value = str;
	return (0);
}

//celle-ci prend soit juste la premiere variable (here != -1)
//soit toutes les variables (here == -1)
//et elle remplace le $VAR par sa valeur dans la token->value
int	handle_var(t_token *token, t_env *env, int here, char *lec)
{
	int	i;

	i = 0;
	while (token->value[i] && !(token->value[i] == '$'
			&& (is_alpha(token->value[i + 1])
				|| token->value[i + 1] == '{' || token->value[i + 1] == '?')))
		i++;
	if (!token->value[i])
		return (0);
	if (token->value[i + 1] == '{')
		i = handle_acc_var(token, env, i);
	else if (is_alpha(token->value[i + 1]))
		i = handle_reg_var(token, env, i);
	else if (token->value[i + 1] == '?')
		i = handle_lec(token, lec, i);
	if (i < 0 || here > -1)
		return (i);
	else
		return (handle_var(token, env, here, lec));
	return (0);
}
