
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

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define GRAY    "\033[90m"
//variable globale
extern int g_err_code;

// big structure

typedef struct s_data
{
    char    *line;
    char    *prompt;
    char    **env_array;
    int last_exit_code;
    struct s_cmd    *cmds;
    struct s_env    *env;
    struct s_token  *token;
}   t_data;

//adding the **env
typedef struct	s_env
{
	char			*var;
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
    struct s_token	*previous;
    struct s_token	*next;
}   t_token;

typedef struct  s_cmd
{
	char			**argv;
	char			*cmd;
	int				infile;
	int				total;
	int				outfile;
	int				built_in;
	pid_t   		pid;
    int             last_exit_code;
	struct s_cmd	*previous;
	struct s_cmd	*next;
}             t_cmd;

//from utils.c
void	print(char *str);
int		is_alpha(int a);
int		is_alphanum(int a);
char	*join(char *s, char *s2);
int		len_str(char *str);

//from env/

//from create_env.c
t_env	*env_item(char *var, int copy); //tested as well, for both copy and no copy
t_env	*free_env(t_env *env); //tested
t_env	*create_env(char **env); //tested
int	update_lst_cmd(t_env *env, t_cmd *cmd);

//from env_utils.c
int		found(char *to_parse, char *to_find); //checked, no error that i can see
char	*copy(char *str); //tested
char	*take_out_plus(char *str, int cut); //tested
int		update_var(t_env *env, char *newvar); //tested

//from env_funcs.c
int		add_env(t_env *env, char *newvar); //tested
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
int		fix_quotes(t_token *token); //fixes previous quote splits if necessary, trims and splits non quotes

//from handle_var.c
char	*replace(char *s, char *var, int where, int next); //tested, no protection
int		handle_var(t_token *token, t_env *env, int here, char *lec); //tested !! no protections for token==NULL, wouldn't get that far in //updated for accolades now, works well (possible future syntax errors :])

//from parsing_utils.c
t_token *new_token(char *value, t_type type, t_token *prev); //tested -- this one does not copy, just grabs;
void	free_tokens(t_token *token);

// I protected 'token' in get quote (i got a segfault without it)
int		get_quote(char *token, char quote); //tested
int		empty_quote(t_token *token);

//from split_token.c
char	**split_once(char *str, int quote); //tested, not protected
int		split_token(t_token *token, int	quote); //tested

//from parsing.c
int		parsing_pt1(t_token *tokens, t_env *env, char *lec); //last sprint

//from syntax

//from syntax_utils.c
void	print_syntax_err(char *str);
int		check_src(int type2, char next);
int		missing_quote(char quote);
int		forbidden(char a);

//from syntax.c
int		spec_check(t_token *token);

//from types

//from split_types.c
int		split_type(t_token *token, int type);

//from types_utils.c
int		get_type_char(char *str, char *types);
int		get_type(char *str, char *types);
void	del_token(t_token *token);
void	cmd_shuffle(t_token *head);
void	shuffle(t_token *token, t_token *where);

//from assign_types.c
int		assign_types(t_token **token);

//in order :: parsing_pt_2 (i'll rename that), fix_quotes, all_cmd_type, spec_check, assign_types and cmd_shuffle

//from extraction

//from str_utils.c
t_cmd	*new_cmd(t_cmd *prev);
void	free_tab(char **tab);
void	free_cmds(t_cmd *head);
void	free_cmds_new(t_cmd *prev, t_cmd *next);

//from get_files.c
int		get_file(t_token *token, t_cmd *cmd, t_env *env, t_data *data);

//from heredoc.c

// pas mal de chgmts ici, creation d'un fork, gestion des signaux et de la var globale
// du coup yaura pas mal de trucs a free en plus je pense, need to check les close aussi
int	get_heredoc_inloop(char *line, t_env *env, int pipefd[2], int code);
int		get_heredoc(char *value, t_env *env, t_data *data);

//from get_commands.c
int		get_command(t_token *token, t_cmd *cmd, char *path); //all good here still

//from extr_utils.c
void	print_bash_err(char *filename, char *err_msg); //err_msg
int		is_built_in(char *function, t_cmd *cmd); //all good
void	ignore_cmd(t_cmd *cmd);
int		str_len_path(char *str);
void	add_count_cmds(t_cmd *cmd); //-- unused rn

//from extraction.c
// assign_cmds is a bit modified to behave accordling to the global var
int		extraction(t_token *token, t_cmd **prev, char *path, t_data *data); //testing... last step

//from built_ins/

//from export_unset.c
int		exportt(char **argv, t_env *env);
int		unset(char **argv, t_env *env);

//from built_ins.c

int		built_in_att1(int func, char **argv, char **envp, t_data *data);

//!!!
//everything above this comment is normed

// created from yours but added line + clear_history and reset pointers to null
int	free_all_things(t_data *data);
// cd and exit
int	ft_exit(t_data *data);
int cd(char **args, t_env *env);

// exec_cmds and exec_builtin alone
void execute_command_or_builtin(t_data *data);
int	exec_builtins(t_cmd *tmp, t_data *data);
int	check_perm(t_data *data);

// from exec_single_cmd
void	exec_single_cmd(t_cmd *tmp, t_data *data);

// from exec_free_close_utils
void	close_all_pipes(int pipe[2]);
void	free_exit(t_data *data, int exit_code);
void	close_fd_new(t_cmd *prev, t_cmd *next);

// from fork_utils
void	wait_for_kids(t_cmd *cmds, t_data *data);
void	check_files(t_cmd *current, int old_pipe[2], int new_pipe[2],
		t_data *data);
void	create_pipe(int new_pipe[2], t_cmd *tmp, t_data *data);
void	update_pipe(t_cmd *current, int old_pipe[2], int new_pipe[2]);
void	builtin_in_fork(t_cmd *tmp, t_data *data);

//from our main_main.c
int isis(char *cat, char *copy);
char	*num_str(int code);

//from autman (signals)
void	sig_handler(int code);
void	sig_handler_heredoc(int code);
void	sig_do_nothing(int code);


// libft utils
void	msg_error(t_data *data, char *str, int code);
void	ft_bzero(void *s, size_t n);
char	*ft_strcat(char *dest, const char *src);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strcpy(char *dest, const char *src);
#endif
