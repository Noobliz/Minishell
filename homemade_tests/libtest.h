#ifndef LIBTEST_H
# define LIBTEST_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

void  disp_env(char **env);
void  free_env(char **env, int end);
int unset(char *line, char ***new_env);
int exporting(char *line, char ***new_env);
char  *get_env(char *var, char **new_env, int len);
int  print_var(char *var, char **new_env);
void  echoing(char *line, char **new_env);
void  print(char *str);
int is(char *line, char *find);
char  *copy_str(char *str);
char **cccv(char **env, char *var, int add, char *del);


#endif
