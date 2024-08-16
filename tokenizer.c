// tokenizer.c
#include "shell.h"

char **split_input(char *input_line) {
    char **token_array = malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *current_token = malloc(MAX_TOKEN_SIZE * sizeof(char));
    
    memset(token_array, 0, sizeof(token_array));

    int char_index, token_index = 0, token_count = 0;

    for (char_index = 0; char_index < strlen(input_line); char_index++) {
        char current_char = input_line[char_index];

        if (current_char == ' ' || current_char == '\n' || current_char == '\t') {
            current_token[token_index] = '\0';
            if (token_index != 0) {
                token_array[token_count] = malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(token_array[token_count++], current_token);
                token_index = 0; 
            }
        } else {
            current_token[token_index++] = current_char;
        }
    }
 
    free(current_token);
    token_array[token_count] = NULL;
    return token_array;
}

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}