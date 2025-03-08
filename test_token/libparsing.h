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

typedef enum e_type
{
    CMD,
    PIPE,
    DIR,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    HEREDOC
}   t_type;

typedef struct s_token
{
    char            *value;   // (ex: "ls", "|", ">", etc.)
    t_type          type;     // (ex : CMD, ARGS, PIPE etc.)
    struct s_token *previous;
    struct s_token *next;
}   t_token;

int	len_str(char *str); //test not needed, supposedly
char	*replace(char *s, char *var, int where); //tested, no protection
int	handle_var(t_token *token, t_env *env, int here); //tested !! no protections for token==NULL, wouldn't get that far in

//careful, check the protection in split_token for quote == end_str
char	**split_once(char *str, int quote); //tested, not protected

t_token *new_token(char *value, t_type type, t_token *prev); //tested -- this one does not copy, just grabs;
int	get_quote(char *token, char quote); //tested
int	split_token(t_token *token, int	quote); //tested
int	add_up(t_token *beg, t_token *end); //tested, frees all

//utils (to make yet)
char  *join(char *s, char *s2); //tested

int	handle_sgquotes(t_token *current); //tested, works fine from what I've seen
int	handle_dbquotes(t_token *current, t_env *env); //tested !! seems good
int	parsing_pt_2(t_token *tokens, t_env *env); //last sprint

char	*del_spaces(char *str); //tested
int	trim_tokens(t_token *token); //tested also

//first part of parsing ready to go, ex ::
//$PATH value '$PATH and  $NUM' more "   $PATH of $NUM    " over there $NUM
//becomes
//Token: [here value] | Type: [0]
//Token: [$PATH and  $NUM] | Type: [0]
//Token: [more] | Type: [0]
//Token: [   here of herealso    ] | Type: [0]
//Token: [over there herealso] | Type: [0]

//the shuffle functions so you can have all commands grouped together in order
void	cmd_shuffle(t_token *head);
void	shuffle(t_token *token, t_token *where);

#endif
