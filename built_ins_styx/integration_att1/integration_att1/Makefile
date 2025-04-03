CC=cc
FLAGS=-Wall -Werror -Wextra -g
SRC=main.c\
main_utils/signal_utils.c\
main_utils/aux.c\
main_utils/init_exec.c\
env/env_utils.c\
env/create_env.c\
env/env_funcs.c\
parsing_tokens/stick_and_split.c\
parsing_tokens/stick_split_utils.c\
parsing_tokens/handle_var.c\
parsing_tokens/parsing_utils.c\
parsing_tokens/split_token.c\
parsing_tokens/parsing.c\
syntax/syntax_utils.c\
syntax/syntax.c\
types/split_types.c\
types/types_utils.c\
types/assign_types.c\
extraction/str_utils.c\
extraction/get_files.c\
extraction/get_commands.c\
extraction/extr_utils.c\
extraction/heredoc_inloop.c\
extraction/heredoc.c\
extraction/extraction.c\
built_ins/built_ins.c\
built_ins/export_unset.c\
built_ins/ft_exit.c\
built_ins/exit_utils.c\
built_ins/cd.c\
built_ins/cd_utils.c\
exec_utils/libft_utils.c\
exec_utils/exec_free_close_utils.c\
exec_utils/fork_utils.c\
execution/exec_builtin_alone.c \
execution/exec_single_cmd.c\
execution/exec_cmds.c\
utils.c\
dead_utils.c

NAME=minishell

all: $(NAME)

$(NAME): $(SRC) libbig.h
	$(CC) $(FLAGS) -o $(NAME) $(SRC) -lreadline

clean:
	rm -f $(NAME)

re: clean all

.PHONY: clean all re
