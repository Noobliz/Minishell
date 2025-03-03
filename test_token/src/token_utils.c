/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:55:14 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/03 14:20:55 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*db_quotes(char	*str, t_env *env)
{
	char	*s;
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (str[i] && str[i] != '\"')
	{
		if (str[i] == '$')
		{
			i++;
			s = get_env(str[i], env);
			j += get_len(s);
			while (is_alphanum(str[i]))
				i++;
		}
		else
		{
			i++;
			j++;
		}
	}
	s = malloc(j + 1);
	if (!s)
		return (NULL);


char	*quote_handler(char *input, t_env *env)
{
	int	i;

	i = 0;
	if (input[0] == '\"')
		return (db_quotes(input, env));
	if (input[0] == '\'')
		return (sg_quotes(input);
	return (input);
}
