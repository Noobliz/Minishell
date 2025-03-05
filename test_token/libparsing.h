/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libparsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:03:20 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/05 13:05:43 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBPARSING_H
# define LIBPARSING_H

# include "libenv.h"

typedef struct	s_token
{
	
}		t_token;

int	len_str(char *str);
char	*replace(char *s, char *var, int where);
int	handle_var(t_token *token, t_env *env, int here);
char	**split_once(char *str, int quote);
t_token *new_token(char *value, t_type type, t_token *prev);
int	get_quote(char *token, char quote);
int	split_token(t_token *token, int	quote);
int	add_up(t_token *beg, t_token *end);


#endif
