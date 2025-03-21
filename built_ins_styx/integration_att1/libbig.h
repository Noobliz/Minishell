#ifndef LIBBIG_H
# define LIBBIG_H

# define _GNU_SOURCE
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>


# include <fcntl.h>

typedef struct	s_env
{
	char	*var;
	struct s_env	*next;
}			t_env;

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

typedef struct  s_cmd
{
	char	**argv;
	char	*cmd;
	int	infile;
	int	total;
	int	outfile;
	int	built_in;
	pid_t   pid;
	struct s_cmd  *previous;
	struct s_cmd  *next;
}             t_cmd;

//from utils.c
void  print(char *str);
int is_alpha(int a);
int is_alphanum(int a);
void	disp_env(t_env *env);
char  *join(char *s, char *s2);
int len_str(char *str);

//from lib_env.h, new_env.c
int	found(char *to_parse, char *to_find); //checked, no error that i can see
char	*copy(char *str); //tested
t_env	*env_item(char *var, int copy); //tested as well, for both copy and no copy
t_env	*free_env(t_env *env); //tested
t_env	*create_env(char **env); //tested
char	*take_out_plus(char *str, int cut); //testing...
int	update_var(t_env *env, char *newvar);//coming up
int	add_env(t_env *env, char *newvar); //tested
void	del_env(t_env *env, char *del); //tested
char	*get_env(char *var, t_env *env); //tested

//from parsing_quotes.c
//var handling
char	*replace(char *s, char *var, int where, int next); //tested, no protection
int	handle_var(t_token *token, t_env *env, int here); //tested !! no protections for token==NULL, wouldn't get that far in //updated for accolades now, works well (possible future syntax errors :])
//quote handling
char	**split_once(char *str, int quote); //tested, not protected
t_token *new_token(char *value, t_type type, t_token *prev); //tested -- this one does not copy, just grabs;
void	free_tokens(t_token *token);
int	get_quote(char *token, char quote); //tested
int	split_token(t_token *token, int	quote); //tested
int	handle_sgquotes(t_token *current); //tested, works fine from what I've seen
int	handle_dbquotes(t_token *current, t_env *env); //tested !! seems good
//does var + quote handling using above funcs
int	parsing_pt_2(t_token *tokens, t_env *env); //last sprint
//fixes previous quote splits if necessary, trims and splits non quotes
char	*del_spaces(char *str); //tested
char	last_char(char *str);
int	split_inner_spaces(t_token *token);
int	add_up(t_token *beg, t_token *end); //tested, frees all
int	fix_quotes(t_token *token);
//resets types
void	delete_token(t_token *token);
void	all_cmd_type(t_token *token);

//from syntax.c
void	print_syntax_err(char *str);
int	check_src(int type2, char next);
int	missing_quote(char quote);
int	forbidden(char a);
int	spec_check(t_token *token);

//from cmd_shuffle.c
//type assigning (and splits if needed)
int	split_type(t_token *token, int type);
int	get_type_char(char *str, char *types);
int	get_type(char *str, char *types);
void	del_token(t_token *token);
int	assign_types(t_token **token);
//shuffles so you can have all commands grouped together in order
void	cmd_shuffle(t_token *head);
void	shuffle(t_token *token, t_token *where);

//in order :: parsing_pt_2 (i'll rename that), fix_quotes, all_cmd_type, spec_check, assign_types and cmd_shuffle

//from extraction.c
t_cmd	*new_cmd(t_cmd *prev);
void	free_tab(char **tab);
void	free_cmds(t_cmd *head);
void	free_cmds_bw(t_cmd *head);
void	free_cmds_new(t_cmd *prev, t_cmd *next);
//tested, all good
void	print_bash_err(char *filename, char *err_msg); //err_msg
int	get_file(t_token *token, t_cmd *cmd, t_env *env);
int	is_built_in(char *function, t_cmd *cmd); //all good
int	find_command(char *paths, char **command); //fixed, added join_path, good now
char	**fill_argv(char *first, t_token *token, char **tab);
char	**get_argv(char *first, t_token *token);
int	get_command(t_token *token, t_cmd *cmd, char *path); //all good here still
int	assign_cmds(t_token *token, t_cmd *cmd, char *path, t_env *env); //all good !!
void	ignore_cmd(t_cmd *cmd);
int	extraction(t_token *token, t_cmd **prev, char *path, t_env *env); //testing... last step
void	add_count_cmds(t_cmd *cmd);

// exec_cmds
void execute_command_or_builtin(t_cmd *cmds, t_env *env, char **envp);

//from built_ins.c
int built_in_att1(int func, char **argv, char **envp, t_env *env);

//from our main_main.c
int isis(char *cat, char *copy);

#endif
