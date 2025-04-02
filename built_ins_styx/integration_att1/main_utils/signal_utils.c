#include "../libbig.h"

void	sig_handler(int code)
{
	(void)code;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

	g_err_code = 130;
}

void	sig_handler_heredoc(int code)
{
	(void)code;
	write(1, "\n", 1);
	g_err_code = 130;
	close(0);
}

void	sig_handler_sigpipe(int code)
{
	(void)code;
	g_err_code = 13;
}

void	sig_do_nothing(int code)
{
	(void)code;
	write(1, "\n", 1);
}

