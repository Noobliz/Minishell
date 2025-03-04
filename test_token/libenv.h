/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libenv.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:24 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/04 19:00:26 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBENV_H
# define LIBENV_H

# include <stdlib.h>
# include <stdio.h>

typedef struct	s_env
{
	char	*var;
	struct s_env	*next;
}			t_env;

int	is_alphanum(char a); //testing not needed
int	found(char *to_parse, char *to_find); //checked, no error that i can see
char	*copy(char *str); //tested
t_env	*env_item(char *var, int copy); //tested as well, for both copy and no copy
t_env	*free_env(t_env *env);
t_env	*create_env(char **env); 
int	add_env(t_env *env, char *newvar);
void	del_env(t_env *env, char *del);
char	*get_env(char *var, t_env *env);

#endif
