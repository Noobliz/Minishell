/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libbig.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:19:33 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/09 13:05:53 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBBIG_H
# define LIBBIG_H

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

extern int	g_err_code;

typedef struct s_data
{
	char			*line;
	char			*prompt;
	char			**env_array;
	int				last_exit_code;
	struct s_cmd	*cmds;
	struct s_env	*env;
	struct s_token	*token;
}				t_data;

typedef struct s_env
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
}			t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				sign;
	struct s_token	*previous;
	struct s_token	*next;
}				t_token;

typedef struct s_cmd
{
	char			**argv;
	char			*cmd;
	int				infile;
	int				outfile;
	int				built_in;
	pid_t			pid;
	struct s_cmd	*previous;
	struct s_cmd	*next;
}				t_cmd;

//from utils.c
void	print(char *str);
int		is_alpha(int a);
int		is_alphanum(int a);
char	*join(char *s, char *s2);
int		len_str(char *str);

//from utils2.c
void	print_err(char *str);

//from env/

//from create_env.c
t_env	*env_item(char *var, int copy);
t_env	*free_env(t_env *env);
t_env	*create_env(char **env);
int		update_lst_cmd(t_env *env, t_cmd *cmd);

//from env_utils.c
int		found(char *to_parse, char *to_find);
char	*copy(char *str);
char	*take_out_plus(char *str, int cut);
int		update_var(t_env *env, char *newvar);

//from env_funcs.c
int		add_env(t_env *env, char *newvar);
void	del_env(t_env *env, char *del);
char	*get_env(char *var, t_env *env);
void	disp_env(t_env *env, int exp);

//from parsing_tokens/

//from stick_split_utils.c
char	*del_spaces(char *str);
char	last_char(char *str);
void	delete_token(t_token *token);

//from split_spaces.c
int		split_and_sign(t_token **token);

//from final.c
int		stick_quotes(t_token *token);
void	final_types(t_token *token);

//from handle_var_utils.c
char	*replace(char *s, char *var, int where, int next);

//fomr handle_var.c
int		handle_var(t_token *token, t_env *env, int here, char *lec);

//from parsing_utils.c
t_token	*new_token(char *value, t_type type, t_token *prev);
void	free_tokens(t_token *token);
int		get_quote(char *token, char quote);
int		empty_quote(t_token *token);
int		is_redir(int type, int is_pipe);

//from split_token.c
char	**split_once(char *str, int quote);
int		split_token(t_token *token, int quote);

//from parsing.c
int		parsing_pt1(t_token *tokens, t_env *env, char *lec);

//from syntax/

//from syntax_utils.c
void	print_syntax_err(char *str);
int		check_src(int type2, int next);
int		missing_quote(char quote);
int		forbidden(char a);

//from syntax.c
int		spec_check(t_token *token);

//from types/

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

//from extraction/

//from str_utils.c
t_cmd	*new_cmd(t_cmd *prev);
void	free_tab(char **tab);
void	free_cmds(t_cmd *head);
void	free_cmds_new(t_cmd *prev, t_cmd *next);

//from get_files.c
int		get_file(t_token *token, t_cmd *cmd);

//from heredoc.c
int		get_heredoc_inloop(char *line, t_env *env, int pipefd[2], int code);
int		get_heredoc(int fd, char *value, t_env *env, t_data *data);

//from get_commands.c
int		get_command(t_token *token, t_cmd *cmd, char *path);

//from extr_utils.c
int		print_bash_err(char *filename, char *err_msg);
int		is_built_in(char *function, t_cmd *cmd);
void	ignore_cmd(t_cmd *cmd);
int		str_len_path(char *str);

//from extraction.c
int		extraction_heredoc(t_token *token, t_cmd **prev, t_data *data);
int		extraction(t_token *token, t_cmd *cmd, t_data *data);

//from built_ins/

//from export_unset.c
int		exportt(char **argv, t_env *env);
int		unset(char **argv, t_env *env);

//from built_ins.c
int		built_in(int func, char **argv, t_data *data, int fork);

//from cd.c and ft_exit.c
int		ft_exit(char **argv, t_data *data, int fork);
int		cd(char **args, t_env **env);

//from exit_utils.c
void	ft_put3str_fd(const char *s1, const char *s2, const char *s3, int fd);
void	ft_putstr_fd(const char *s, int fd);
int		is_numeric(const char *str);

//from cd_utils.c
char	*ft_strdup(char *s);
char	*get_env_value(t_env *env, char *name);
char	*create_env_string(char *name, char *value);
int		replace_if_exists(t_env *env, char *name, char *new_var);
int		add_to_env(t_env **env, char *new_var);

//from execution/

//from exec_builtin_alone.c, exec_cmds.c
void	execute_command_or_builtin(t_data *data);
int		exec_builtins(t_cmd *tmp, t_data *data);
int		check_perm(t_data *data);

//from exec_single_cmd.c
void	exec_single_cmd(t_cmd *tmp, t_data *data);

//from exec_utils/

//from exec_free_close_utils.c
void	close_all_pipes(int pipe[2]);
void	free_exit(t_data *data, int exit_code);
void	close_fd_new(t_cmd *prev, t_cmd *next);

//from fork_utils.c
void	wait_for_kids(t_cmd *cmds, t_data *data);
void	check_files(t_cmd *current, int old_pipe[2], int new_pipe[2],
			t_data *data);
void	create_pipe(int new_pipe[2], t_cmd *tmp, t_data *data);
void	update_pipe(t_cmd *current, int old_pipe[2], int new_pipe[2]);
void	builtin_in_fork(t_cmd *tmp, t_data *data);

// libft utils
void	msg_error(t_data *data, char *str, int code);
char	*ft_strcat(char *dest, const char *src);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strcpy(char *dest, const char *src);
int		check_empty(t_token *token, char *str);

//from main_utils/

//from aux.c
int		isis(char *cat, char *copy);
char	*num_str(int code);
char	**env_to_array(t_env *env);
int		free_all_things(t_data *data);

//from init_exec.c
char	*get_prompt(t_env *env);
int		making_tokens(t_token **token, t_env *env, char *lec);
int		data_init(t_data *data, char **envp);
int		reset_readline(t_data *data);
int		exec_and_co(t_data *data, int check);

//from signal_utils.c
void	sig_handler(int code);
void	sig_handler_heredoc(int code);
void	sig_do_nothing(int code);
void	sig_handler_sigpipe(int code);

//from dead_utils.c
void	print_tokens(t_token *tokens);
void	print_cmds(t_cmd *cmd);
void	all_cmd_type(t_token **tokens); //unused rn

#endif
