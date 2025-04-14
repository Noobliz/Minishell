/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:27:38 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/14 13:53:36 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while ((unsigned char)s1[i] && (unsigned char)s2[i]
		&& s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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

	if (!args[1] || ft_strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "~/") == 0)
		target = get_env_value(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (target)
			printf("%s\n", target);
		else
			print_err("cd: OLPWD not set\n");
	}
	else
		target = args[1];
	return (target);
}

static int	update_pwd_vars(t_env **env, char *oldpwd, char *target)
{
	char	*newpwd;
	char	*tmp;
	int		res;

	newpwd = getcwd(NULL, 0);
	res = 0;
	if (!newpwd)
	{
		perror("cd: getcwd");
		tmp = join(get_env("PWD", *env), "/");
		if (!tmp)
			return (-1);
		newpwd = join(tmp, target);
		free(tmp);
		if (!newpwd)
			return (-1);
		res = -1;
	}
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);
	set_env_var(env, "1PWD", newpwd);
	free(newpwd);
	return (res);
}

int	cd(char **args, t_env **env)
{
	char	*target;
	char	*value;
	char	*oldpwd;

	if (!env || !*env)
		return (ft_putstr_fd("env not found\n", 2), -2);
	value = get_env_value(*env, "PWD");
	if (!value)
		value = "";
	target = get_target(args, *env);
	oldpwd = ft_strdup(value);
	if (!oldpwd)
		return (perror("cd : malloc"), 12);
	if (!target || chdir(target) != 0)
	{
		if (!args[1] || args[1][0] != '-')
			perror("cd");
		free(oldpwd);
		return (1);
	}
	if (update_pwd_vars(env, oldpwd, target) == -1)
		return (free(oldpwd), 12);
	free(oldpwd);
	return (0);
}
