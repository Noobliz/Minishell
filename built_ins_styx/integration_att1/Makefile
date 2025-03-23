CC=cc
FLAGS=-Wall -Werror -Wextra
SRC=main_main.c\
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
extraction.c\
built_ins/built_ins.c\
built_ins/export_unset.c\
exec_cmds2.c\
utils.c
NAME=bi_minishell

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) -o $(NAME) $(SRC) -lreadline

clean:
	rm -f $(NAME)

re: clean all

.PHONY: clean all re
