/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:27:38 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:28:05 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	set_env_var(t_env **env, char *name, char *value)
{
	char	*new_var;

	new_var = create_env_string(name, value);
	if (!new_var)
		return (-1);
	if (replace_if_exists(*env, name, new_var))
		return (0);
	if (add_to_env(env, new_var) == -1)
	{
		free(new_var);
		return (-1);
	}
	return (0);
}

static char	*get_target(char **args, t_env *env)
{
	char	*target;

	if (!args[1] || strcmp(args[1], "~") == 0)
		target = get_env_value(env, "HOME");
	else if (strcmp(args[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (target)
			printf("%s\n", target);
	}
	else
		target = args[1];
	return (target);
}

static int	update_pwd_vars(t_env **env, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (-1);
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);
	set_env_var(env, "1PWD", newpwd);
	free(newpwd);
	return (0);
}

int	cd(char **args, t_env **env)
{
	char	*target;
	char	*oldpwd;

	if (!env || !*env)
		return (printf("env not found\n"), -1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("cd"), -1);
	target = get_target(args, *env);
	if (!target || chdir(target) != 0)
	{
		printf("target not found\n");
		free(oldpwd);
		return (-1);
	}
	if (update_pwd_vars(env, oldpwd) == -1)
		return (free(oldpwd), -1);
	free(oldpwd);
	return (0);
}
