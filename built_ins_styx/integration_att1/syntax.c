#include "libbig.h"

//for printing syntax errors, can change to "minishell" instead of "bash" if necessary
void	print_syntax_err(char *str)
{
	print("bash: syntax error near unexpected token \'");
	print(str);
	print("\'\n");
}
//in this one i check what character i'm supposed to display as the unexpected token (i've checked every possible situation i could think of)
int	check_src(int type2, char next)
{
	if (type2 == PIPE)
		print_syntax_err("|");
	else if (type2 == REDIR_IN)
		print_syntax_err("<");
	else if (type2 == HEREDOC)
		print_syntax_err("<<");
	else if (type2 == APPEND)
		print_syntax_err(">>");
	else if (type2 == REDIR_OUT && next == '|')
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
//error message for a missing matching quote (used in parsing_quotes.c, when checking for sg and db quotes)
int	missing_quote(char quote)
{
	print("bash : missing matching '");
	write(1, &quote, 1);
	print("'\n");
	return (-2);
}
//this function does every syntax check to do with special characters, both authorized and not. it checks that a REDIR_IN or other has a file given, and that a PIPE is not at the beginning or end of the line;
int	spec_check(t_token *token)
{
	int	type = -1;
	int	type2;
	int	i;

	if (token && token->value[0] == '|')
        	return (check_src(PIPE, 'a'));
	while (token)
	{
		if (token->type != IGNORE)
		{
			i = 0;
			while (token->value && token->value[i] != '\0')
			{
				if (forbidden(token->value[i]))
					return (-2);
				type2 = get_type_char(&token->value[i], "<|>");
				if (type2 != -1)
				{
					if (type != -1 && type != PIPE)
						return (check_src(type2, token->value[i + 1]));
					type = type2;
					if (type2 == APPEND || type2 == HEREDOC)
						i++;
				}
				else
					type = -1;
				i++;
			}
		}
		else
			type = -1;
		if (!token->next && type2 != -1)
			print_syntax_err("newline");
		token = token->next;
	}
	return (0);
}
