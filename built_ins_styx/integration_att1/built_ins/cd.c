

#include "../libbig.h"


char *get_env_value(t_env *env, char *name)
{
	size_t len = len_str(name);

	while (env)
	{
		if (ft_strncmp(env->var, name, len) == 0 && env->var[len] == '=')
			return (env->var + len + 1);
		env = env->next;
	}
	return (NULL);
}

int set_env_var(t_env *env, char *name, char *value)
{
	t_env *tmp;
	size_t len;
	char *new_var;
	t_env	*new;
	
	tmp = env;
	len = len_str(name);
	new_var = malloc(len + len_str(value) + 2);
	if (!new_var)
		return (-1);
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	while (tmp)
	{
		if (ft_strncmp(tmp->var, name, len) == 0 && tmp->var[len] == '=')
		{
			free(tmp->var);
			tmp->var = new_var;
			return (0);
		}
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free(new_var);
		return (-1);
	}
	new->var = new_var;
	new->next = NULL;
	tmp = env;
	if (!tmp)
		env = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}


int cd(char **args, t_env *env)
{
	char *target;
	char *oldpwd;
	t_env	*tmp;

	target = NULL;
	oldpwd = getcwd(NULL, 0);
	tmp = env;
	if (!env)
	{
		printf("env not found\n");
		return (-1);
	}
	if (!oldpwd)
	{
		perror("cd");
		return (-1);
	}

	if (!args[1] || strcmp(args[1], "~") == 0)
		target = get_env_value(tmp, "HOME");

	else if (strcmp(args[1], "-") == 0)
	{
		target = get_env_value(tmp, "OLDPWD");
		if (target)
			printf("%s\n", target);
	}
	else
		target = args[1];

	if (!target || chdir(target) != 0)
	{
		printf("target not found\n");
		free(oldpwd);
		return (-1);
	}

	char *newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		set_env_var(tmp, "OLDPWD", oldpwd);
		set_env_var(tmp, "PWD", newpwd);
		free(newpwd);
	}
	free(oldpwd);
	return (0);
}

