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

typedef struct	s_env
{
	char	*var;
	struct s_env	*next;
}			t_env;

//int	is_alphanum(int a); //testing not needed
int	found(char *to_parse, char *to_find); //checked, no error that i can see
char	*copy(char *str); //tested
t_env	*env_item(char *var, int copy); //tested as well, for both copy and no copy

//common functions for outside use, env related ::
t_env	*free_env(t_env *env); //tested
t_env	*create_env(char **env); //tested
char	*take_out_plus(char *str, int cut); //testing...
int	update_var(t_env *env, char *newvar);//coming up
int	add_env(t_env *env, char *newvar); //tested
void	del_env(t_env *env, char *del); //tested
char	*get_env(char *var, t_env *env); //tested

# include <unistd.h>
//from utils.h
void  print(char *str);
int is_alpha(int a);
int is_alphanum(int a);
void	disp_env(t_env *env);
char  *join(char *s, char *s2);
int len_str(char *str);

#endif
