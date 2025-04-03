CC=cc
FLAGS=-Wall -Werror -Wextra

see_dir: dirs.c
	$(CC) $(FLAGS) -o see_dir dirs.c

is_dir: file_type.c
	$(CC) $(FLAGS) -o is_dir file_type.c

new_pwd: new_pwd.c
	$(CC) $(FLAGS) -o new_pwd new_pwd.c

tty_test: tty_test.c
	$(CC) $(FLAGS) -o tty_test tty_test.c

readliner: readline.c
	$(CC) $(FLAGS) -o readliner readline.c -lreadline

clean:
	rm -f see_dir is_dir new_pwd tty_test readliner

all : see_dir is_dir new_pwd tty_test readliner

.PHONY: clean all
