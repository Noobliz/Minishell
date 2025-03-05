/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:57:10 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/04 19:00:05 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libparsing.h"
#include <stdio.h>

void	disp_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->var);
		env = env->next;
	}
}

int main(int argc, char **argv, char **env)
{
	t_env	*n_env;
	char *str;

	if (argc > 1 || argv[0][0] == '\0')
		return (0);
	str = replace("some word here", "other word", 5);
	if (str)
	{
		printf("here new string :: %s", str);
		free(str);
	}
	return (0);
}
