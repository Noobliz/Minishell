
NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCDIR = src
OBJDIR = obj
LIBFTDIR = libft

SRC = $(SRCDIR)/token.c
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Libft
LIBFT = $(LIBFTDIR)/libft.a
LIBFT_INC = -I $(LIBFTDIR)
LIBFT_FLAG = -L $(LIBFTDIR) -lft

# Readline
READLINE_INC = -I /usr/include
READLINE_LIB = -L /usr/lib -lreadline

RM = rm -rf
MKDIR = mkdir -p

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FLAG) $(READLINE_LIB) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(OBJDIR) # Assure-toi que le dossier obj/ existe
	$(CC) $(CFLAGS) -I includes $(LIBFT_INC) $(READLINE_INC) -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	$(RM) $(OBJ)
	make -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
