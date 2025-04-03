/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:58:29 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 19:58:31 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//parcourt la variable environnement to_parse
//cherche la variable donnee to_find
int	found(char *to_parse, char *to_find)
{
	int	i;

	i = 0;
	while (to_parse[i] != '=' && is_alphanum(to_find[i]))
	{
		if (to_parse[i] != to_find[i])
			return (0);
		i++;
	}
	if (to_parse[i] == '=' && !is_alphanum(to_find[i]))
		return (1);
	return (0);
}

//copy/strdup
char	*copy(char *str)
{
	char	*s;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
		i++;
	s = malloc(i + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

//and the part two where i fill the new string;
char	*take_out_pt2(char *s, char *str, int cut)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i] && (i == 0 || str[i - 1] != '='))
	{
		if (str[i] != '+' && !cut)
		{
			s[len] = str[i];
			len++;
		}
		i++;
	}
	while (str[i])
	{
		s[len] = str[i];
		len++;
		i++;
	}
	s[len] = '\0';
	return (s);
}

//this one returns :: on VAR=var, (1 or 0) ==> a copied string,
//on VAR+=var (0) ==> VAR=var, on VAR+=var (1) ==> var;
char	*take_out_plus(char *str, int cut)
{
	char	*s;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '+')
			len++;
		i++;
	}
	if (cut && len != i)
		len = 0;
	else
		cut = 0;
	while (str[i])
	{
		len++;
		i++;
	}
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	return (take_out_pt2(s, str, cut));
}
