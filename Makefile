CC=cc
FLAGS=-Wall -Werror -Wextra

see_dir: dirs.c
	$(CC) $(FLAGS) -o see_dir dirs.c

is_dir: file_type.c
	$(CC) $(FLAGS) -o is_dir file_type.c

clean:
	rm -f see_dir is_dir

all : see_dir is_dir

.PHONY: clean
