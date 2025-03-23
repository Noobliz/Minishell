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
char  *join(char *s, char *s2);
int len_str(char *str);

//from env/

//from create_env.c
t_env	*env_item(char *var, int copy); //tested as well, for both copy and no copy
t_env	*free_env(t_env *env); //tested
t_env	*create_env(char **env); //tested

//from env_utils.c
int	found(char *to_parse, char *to_find); //checked, no error that i can see
char	*copy(char *str); //tested
char	*take_out_plus(char *str, int cut); //tested
int	update_var(t_env *env, char *newvar); //tested

//from env_funcs.c
int	add_env(t_env *env, char *newvar); //tested
void	del_env(t_env *env, char *del); //tested
char	*get_env(char *var, t_env *env); //tested
void	disp_env(t_env *env);

//from parsing_tokens/

//from stick_split_utils.c
char	*del_spaces(char *str); //tested
char	last_char(char *str);
void	delete_token(t_token *token);
void	all_cmd_type(t_token *token); //resets types

//from stick_and_split.c
int	fix_quotes(t_token *token); //fixes previous quote splits if necessary, trims and splits non quotes

//from handle_var.c
char	*replace(char *s, char *var, int where, int next); //tested, no protection
int	handle_var(t_token *token, t_env *env, int here); //tested !! no protections for token==NULL, wouldn't get that far in //updated for accolades now, works well (possible future syntax errors :])

//from parsing_utils.c
t_token *new_token(char *value, t_type type, t_token *prev); //tested -- this one does not copy, just grabs;
void	free_tokens(t_token *token);
int	get_quote(char *token, char quote); //tested
int	empty_quote(t_token *token);

//from split_token.c
char	**split_once(char *str, int quote); //tested, not protected
int	split_token(t_token *token, int	quote); //tested

//from parsing.c
int	parsing_pt1(t_token *tokens, t_env *env); //last sprint

//from syntax

//from syntax_utils.c
void	print_syntax_err(char *str);
int	check_src(int type2, char next);
int	missing_quote(char quote);
int	forbidden(char a);

//from syntax.c
int	spec_check(t_token *token);

//from types

//from split_types.c
int	split_type(t_token *token, int type);

//from types_utils.c
int	get_type_char(char *str, char *types);
int	get_type(char *str, char *types);
void	del_token(t_token *token);
void	cmd_shuffle(t_token *head);
void	shuffle(t_token *token, t_token *where);

//from assign_types.c
int	assign_types(t_token **token);

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

//from built_ins/

//from export_unset.c
int	exportt(char **argv, t_env *env);
int	unset(char **argv, t_env *env);

//from built_ins.c
int built_in_att1(int func, char **argv, char **envp, t_env *env);

//from our main_main.c
int isis(char *cat, char *copy);

#endif
