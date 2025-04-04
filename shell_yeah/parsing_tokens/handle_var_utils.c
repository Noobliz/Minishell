/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_utils.c                                 :+:      :+:    :+:   */
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
