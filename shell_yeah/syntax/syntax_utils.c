/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:08:11 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/03 13:01:00 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//for printing syntax errors
//can change to "minishell" instead of "bash" if necessary
void	print_syntax_err(char *str)
{
	print_err("\033[35myou shell not pass:\033[0m ");
	print_err("syntax error near unexpected token \'");
	print_err(str);
	print_err("\'\n");
}

//in this one i check what character i'm supposed to display
//as the unexpected token
//(i've checked every possible situation i could think of)
int	check_src(int type2, int next)
{
	if (type2 == PIPE)
		print_syntax_err("|");
	else if (type2 == REDIR_IN)
		print_syntax_err("<");
	else if (type2 == HEREDOC)
		print_syntax_err("<<");
	else if (type2 == APPEND)
		print_syntax_err(">>");
	else if (type2 == REDIR_OUT && next == PIPE)
		print_syntax_err(">|");
	else
		print_syntax_err(">");
	return (-2);
}

//forbidden characters
int	forbidden(char a)
{
	if (a == ';')
		print_syntax_err(";");
	else if (a == '&')
		print_syntax_err("&");
	else
		return (0);
	return (1);
}

//error message for a missing matching quote or acc
//(used in parsing for quotes or acc variables)
int	missing_quote(char quote)
{
	print_err("\033[35myou shell not pass:\033[0m missing matching '");
	write(2, &quote, 1);
	print_err("'\n");
	return (-2);
}
