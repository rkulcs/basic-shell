#include "interface.h"

void display_prompt()
{
    printf("shell> ");
}

char* get_command()
{
    display_prompt();
    char* user_input = (char*) malloc(MAX_INPUT_LENGTH * sizeof(char));
    fgets(user_input, MAX_INPUT_LENGTH, stdin);

    // Remove "\n" from user input
    user_input[strlen(user_input) - 1] = '\0';
    user_input = realloc(user_input, strlen(user_input) + 1);

    return user_input;
}