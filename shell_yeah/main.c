/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:17:22 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 14:50:14 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbig.h"

int	g_err_code;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		check;

	if (argc > 1 || argv[1] || !data_init(&data, envp))
		return (0);
	while (data.line)
	{
		add_history(data.line);
		data.token = new_token(data.line, CMD, NULL);
		if (!data.token)
			return (free_all_things(&data));
		check = making_tokens(&data.token, data.env, num_str(data.last_exit_code));
		if (check == -1)
			return (free_all_things(&data));
		if (check != -2 && extraction(data.token, &data.cmds, get_env("PATH",
					data.env), &data) < 0)
			return (free_all_things(&data));
		if (!exec_and_co(&data, check))
			return (0);
		if (!reset_readline(&data))
			return (0);
	}
	write(1, "exit\n", 5);
	return (free_all_things(&data));
}
