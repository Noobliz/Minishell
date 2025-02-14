CC=cc
FLAGS=-Wall -Werror -Wextra

see_dir: dirs.c
	$(CC) $(FLAGS) -o see_dir dirs.c

clean:
	rm -f see_dir

.PHONY: clean
