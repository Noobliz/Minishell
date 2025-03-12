#ifndef LIBEXTRACT_H
# define LIBEXTRACT_H
# include "libparsing.h"
# include <errno.h>

typedef struct  s_cmd
{
	char	**argv;
	char	*cmd;
	int	infile;
	int	outfile;
	(*built_in)(char **);
	struct s_cmd  *next;
}             t_cmd;

t_cmd	*new_cmd(void);
void	free_tab(char **tab);
void	free_cmds(t_cmd *head);
void	print_bash_err(char *filename, char *err_msg);
int	get_file(t_token *token, t_cmd *cmd);
int	is_built_in(char *function, t_cmd *cmd);
int	find_command(char *paths, char **command);
char	**fill_argv(char *first, t_token *token, char **tab);
char	**get_argv(char *first, t_token *token);
int	get_command(t_token *token, t_cmd *cmd, char *path);
int	assign_cmds(t_token *token, t_cmd *cmd, char *path);
void	ignore_cmd(t_cmd *cmd);
int	extraction(t_token *token, t_cmd **head, char *path);

#endif
