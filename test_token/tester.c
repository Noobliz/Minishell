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

#include "libenv.h"

void	disp_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->var);
		env = env->next;
	}
}

int main(void)
{
	t_env	*n_env;

	/*n_env = create_env(env);
	if (!n_env)
		return (0);
	disp_env(n_env);
	free_env(n_env);
	return (0);*/
	n_env = env_item("PATH7=value", 0);
	if (n_env)
	{
		printf("have an item with var :: %s\n", n_env->var);
		free(n_env);
	}
	n_env->next = env_item("PATH7=value", 0);
	if (n_env)
	{
		printf("have an item with var :: %s\n", n_env->var);
		free(n_env);
	}
	return (0);
}
