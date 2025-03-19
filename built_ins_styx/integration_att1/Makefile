CC=cc
FLAGS=-Wall -Werror -Wextra
SRC=main_main.c\
new_env.c\
parsing_quotes.c\
syntax.c\
cmd_shuffle.c\
extraction.c\
built_ins.c\
utils.c
NAME=bi_minishell

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) -o $(NAME) $(SRC) -lreadline

clean:
	rm -f $(NAME)

re: clean all

.PHONY: clean all re
