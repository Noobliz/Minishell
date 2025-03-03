
//#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

typedef enum e_type
{
    CMD,
    ARG,
    PIPE,
    DIR,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    HEREDOC
}   t_type;

typedef struct s_token
{
    char            *value;   // (ex: "ls", "|", ">", etc.)
    t_type          type;     // (ex : CMD, ARGS, PIPE etc.)
    struct s_token *previous;
    struct s_token *next;
}   t_token;

t_token *create_token(char *value, t_type type)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return(NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->next = NULL;
    new->previous = NULL;
	return(new);
}
//HY ft_strdup ? on pourrait free le char ** en gardant les char *pointers, eviter les protections du malloc

// Ici c'est juste la creation du maillon avec un pointeur vers next et un autre vers previous
void	add_token(t_token **head, char *value, t_type type)
{
	t_token *new = create_token(value, type);
	t_token *tmp;

	if(!new)
		return;
	if(!*head)
		*head = new;
	else
	{
		tmp = *head;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new;
        new->previous = tmp;
	}
}

t_type get_token_type(char *word, t_token *current)
{
    if (!ft_strcmp(word, "|"))
        return PIPE;
    if (!ft_strcmp(word, "<"))
        return REDIR_IN;
    if (!ft_strcmp(word, ">"))
        return REDIR_OUT;
    if (!ft_strcmp(word, ">>"))
        return APPEND;
    if (!ft_strcmp(word, "<<"))
        return HEREDOC;

// là on récupère le type de la commande avant word, y'a un décalage entre la liste chainée et word, en gros current->type ca correspond au word -1
// avec ça on détermine si word est un arg ou un fichier (oui j'ai give up l'anglais sorry)
    if (current)
    {
        //printf("current value = %s\n word = %s\n", current->value, word);
        if (current->type == 0 || current->type == ARG)
            return ARG;
        
        if (current->type == REDIR_OUT || current->type == REDIR_IN ||
            current->type == APPEND || current->type == HEREDOC)
            return DIR;
    }

    return CMD;
}
// building the chained list with the name and the type of input
t_token *parse_input(char *input)
{
    t_token *tokens = NULL;
    t_token *current = NULL;
    char **words = ft_split(input, ' ');
    int i = 0;

    if (!words)
        return (NULL);

    while (words[i])
    {
        t_type type = get_token_type(words[i], current);
        add_token(&tokens, words[i], type);
        if (!current)
            current = tokens;
        else
            current = current->next;

        free(words[i]);
        i++;
    }
    free(words);
    
    return tokens;
}



void free_tokens(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

// print also the previous to be sure everything is correct
void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        if(tokens && tokens->previous)
            printf("previous token value = %s previous token type = %d\n", tokens->previous->value, tokens->previous->type);
        tokens = tokens->next;
    }
}

// I tested with the input :  ls -l | grep minishell > output.txt
// the goal was to store everything with correct type
int main(void)
{
    char *input;
    t_token *tokens;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);

        tokens = parse_input(input);
        print_tokens(tokens);

        free(input);
    }
    free_tokens(tokens);
    return 0;
}
