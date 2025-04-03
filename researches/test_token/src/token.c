
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
    if (!strcmp(word, "|"))
        return PIPE;
    if (!strcmp(word, ">"))
        return REDIR_OUT;
    if (!strcmp(word, ">>"))
        return APPEND;
    if (!strcmp(word, "<"))
        return REDIR_IN;
    if (!strcmp(word, "<<"))
        return HEREDOC;

    if (current)
    {
        if (current->type == REDIR_OUT || current->type == REDIR_IN ||
            current->type == APPEND || current->type == HEREDOC)
            return DIR;
    }

    return CMD;
}
// char **split_special_chars(char *input)
// {
//     char **result;
//     int i, j, k, len;
//     char buffer[1024]; // to extract words


//     i = 0;
//     k = 0;
//     len = ft_strlen(input);

//     result = malloc(sizeof(char *) * (len * 2 + 1)); // making a huge malloc but idk how to do otherwise
//     if (!result)
//         return (NULL);
//     while (i < len)
//     {
//         j = 0;
//         while (input[i] && input[i] != ' ' && input[i] != '<' && input[i] != '>' && input[i] != '|')
//         {
//             buffer[j++] = input[i++];
//         }
//         if (j > 0) // if characters were cumulated, we stock them
//         {
//             buffer[j] = '\0';
//             result[k++] = ft_strdup(buffer);
//         }

//         if (input[i] == '<' || input[i] == '>' || input[i] == '|')
//         {
//             if (input[i + 1] == input[i]) //  `>>` or `<<` question : if the user write a doble pipe, what should we do ? :'D 
//             {
//                 buffer[0] = input[i];
//                 buffer[1] = input[i + 1];
//                 buffer[2] = '\0';
//                 i++;
//             }
//             else
//             {
//                 buffer[0] = input[i];
//                 buffer[1] = '\0';
//             }
//             result[k++] = ft_strdup(buffer);
//             i++;
//         }
//         else if (input[i] == ' ')
//             i++;
//     }

//     result[k] = NULL;
//     return result;
// }

//TEST SPLIT WITH VARIOUS SEPARATORS and a bit less ugly than previous one

int is_separator(char c, const char *separators)
{
    int i = 0;
    while (separators[i])
    {
        if (c == separators[i])
            return (1);
        i++;
    }
    return (0);
}

int count_tokens(const char *str, const char *separators)
{
    int count = 0;
    int i = 0;

    while (str[i])
    {
        while (str[i] && is_separator(str[i], separators)) 
            i++;
        if (str[i])
        {
            count++;
            while (str[i] && !is_separator(str[i], separators))
                i++;
        }
        if (str[i] && is_separator(str[i], separators))
        {
            count++;
            i++;
        }
    }
    return count;
}

char **ft_split_multi(const char *str, const char *separators)
{
    char **result;
    int i = 0, k = 0, word_start;
    int num_tokens = count_tokens(str, separators);

    result = malloc(sizeof(char *) * (num_tokens + 1));
    if (!result)
        return (NULL);

    while (str[i])
    {
        while(str[i] && str[i] == ' ') //we jump spaces because we don't want tokens with space
            i++;
        if (is_separator(str[i], separators)) 
        {
            result[k++] = ft_substr(str, i, 1); 
            i++;
        }
        else
        {
            word_start = i;
            while (str[i] && !is_separator(str[i], separators)) 
                i++;
            result[k++] = ft_substr(str, word_start, i - word_start);
        }
    }
    result[k] = NULL;
    return result;
}



// building the chained list with the name and the type of input

t_token *parse_input(char *input)
{
    t_token *tokens = NULL;
    t_token *current = NULL;
    char **words = ft_split_multi(input, "<|> "); 
    int i = 0;

    if (!words)
        return (NULL);
    // while(words[i])
    // {
    //     printf("words[%d] = %s\n", i, words[i]);
    //     i++;
    // }
    // i = 0;
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
        // if(tokens && tokens->previous)
        //     printf("previous token value = %s previous token type = %d\n", tokens->previous->value, tokens->previous->type);
        tokens = tokens->next;
    }
}


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
