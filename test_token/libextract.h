#ifndef LIBEXTRACT_H
# define LIBEXTRACT_H
# include "libparsing.h"
# include <errno.h>
# include <fcntl.h>
# include <string.h>

typedef struct  s_cmd
{
	char	**argv;
	char	*cmd;
	int	infile;
	int	outfile;
	int	(*built_in)(char **);
	struct s_cmd  *previous;
	struct s_cmd  *next;
}             t_cmd;

t_cmd	*new_cmd(t_cmd *prev);
void	free_tab(char **tab);
void	free_cmds(t_cmd *head);
void	free_cmds_bw(t_cmd *head);
void	free_cmds_new(t_cmd *prev, t_cmd *next);
//tested, all good
void	print_bash_err(char *filename, char *err_msg); //err_msg
int	get_file(t_token *token, t_cmd *cmd);
//all tests conclude well, no issues so far (good error messages (majs though), opened fds, created files etc.)
int	is_built_in(char *function, t_cmd *cmd); //all good
int	find_command(char *paths, char **command); //fixed, added join_path, good now
char	**fill_argv(char *first, t_token *token, char **tab);
char	**get_argv(char *first, t_token *token);
int	get_command(t_token *token, t_cmd *cmd, char *path); //all good here still
int	assign_cmds(t_token *token, t_cmd *cmd, char *path); //all good !!
void	ignore_cmd(t_cmd *cmd);
int	extraction(t_token *token, t_cmd **prev, char *path); //testing... last step

//all good !! starting hybrid tests now

#endif
