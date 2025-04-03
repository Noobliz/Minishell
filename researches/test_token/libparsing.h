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
# include <unistd.h>

typedef enum e_type
{
    CMD,
    PIPE,
    DIR,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    HEREDOC,
    IGNORE
}   t_type;

typedef struct s_token
{
    char            *value;   // (ex: "ls", "|", ">", etc.)
    t_type          type;     // (ex : CMD, ARGS, PIPE etc.)
    struct s_token *previous;
    struct s_token *next;
}   t_token;

int	len_str(char *str); //test not needed, supposedly
char	*replace(char *s, char *var, int where, int next); //tested, no protection
int	handle_var(t_token *token, t_env *env, int here); //tested !! no protections for token==NULL, wouldn't get that far in //updated for accolades now, works well (possible future syntax errors :])

//careful, check the protection in split_token for quote == end_str
char	**split_once(char *str, int quote); //tested, not protected

t_token *new_token(char *value, t_type type, t_token *prev); //tested -- this one does not copy, just grabs;
int	get_quote(char *token, char quote); //tested
int	split_token(t_token *token, int	quote); //tested
int	add_up(t_token *beg, t_token *end); //tested, frees all
void	all_cmd_type(t_token *token);

//utils (to make yet)
char  *join(char *s, char *s2); //tested

int	handle_sgquotes(t_token *current); //tested, works fine from what I've seen
int	handle_dbquotes(t_token *current, t_env *env); //tested !! seems good
int	parsing_pt_2(t_token *tokens, t_env *env); //last sprint

char	*del_spaces(char *str); //tested
int	trim_split_tokens(t_token *token); //tested also

//the shuffle functions so you can have all commands grouped together in order
void	cmd_shuffle(t_token *head);
void	shuffle(t_token *token, t_token *where);

//here the type assigning (and splits ofc)
int	split_type(t_token *token, int type);
int	get_type_char(char *str, char *types);
int	get_type(char *str, char *types);
void	del_token(t_token *token);
int	assign_types(t_token **token);

char	last_char(char *str);
int	split_inner_spaces(t_token *token);
int	fix_quotes(t_token *token);
//all tested and ready to go !! haven't found an issue, but if you do don't hesitate.
//in order :: parsing_pt_2 (i'll rename that), trim_split_token, assign_types and cmd_shuffle

//Next up syntax errors ! ready to go, will be adjusted on the go if and when necessary !
void	print(char *str);
void	print_syntax_err(char *str);
int	check_src(int type2, char next);
int	missing_quote(char quote);
int	forbidden(char a);
int	spec_check(t_token *token);

#endif
