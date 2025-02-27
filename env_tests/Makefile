CC=cc
FLAGS=-Wall -Werror -Wextra

envi: trying.c
	$(CC) $(FLAGS) -o envi trying.c -lreadline

clean:
	rm -f envi

all: envi

.PHONY: clean all
