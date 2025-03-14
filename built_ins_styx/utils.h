#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include "libenv.h"

void  print(char *str);
int is_alpha(int a);
int is_alphanum(int a);
void	disp_env(t_env *env);

#endif
