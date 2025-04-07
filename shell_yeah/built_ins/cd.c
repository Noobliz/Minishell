/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:27:38 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/07 16:19:55 by lisux            ###   ########.fr       */
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

static int	update_pwd_vars(t_env **env, char *oldpwd, char *target)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd");
		set_env_var(env, "OLDPWD", oldpwd);
		set_env_var(env, "PWD", target);
		set_env_var(env, "1PWD", target);
		return (-1);
	}
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);
	set_env_var(env, "1PWD", newpwd);
	free(newpwd);
	return (0);
}

int	cd(char **args, t_env **env)
{
	char	*target;
	char	*value;
	char	*oldpwd;

	if (!env || !*env)
		return (ft_putstr_fd("env not found\n", 2), -1);
	value = get_env_value(*env, "PWD");
	if (!value)
		return (ft_putstr_fd("target not found\n", 2), -1);
	target = get_target(args, *env);
	oldpwd = strdup(value);
	if (!oldpwd)
		return (perror("cd : malloc"), -1);
	if (!target || chdir(target) != 0)
	{
		ft_putstr_fd("target not found\n", 2);
		free(oldpwd);
		return (-1);
	}
	if (update_pwd_vars(env, oldpwd, target) == -1)
		return (free(oldpwd), -1);
	free(oldpwd);
	return (0);
}
// int	cd(char **args, t_env **env)
// {
// 	char	*target;
// 	char	*oldpwd;

// 	if (!env || !*env)
// 		return (printf("env not found\n"), -1);
// 	oldpwd = getcwd(NULL, 0);
// 	target = get_target(args, *env);
// 	if (!target || chdir(target) != 0)
// 	{
// 		perror("cd");
// 		if(oldpwd)
// 			free(oldpwd);
// 		return (-1);
// 	}
// 	if (!oldpwd)
// 		return (perror("cd"), -1);
// 	if (update_pwd_vars(env, oldpwd) == -1)
// 		return (free(oldpwd), -1);
// 	free(oldpwd);
// 	return (0);
// }