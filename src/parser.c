#include "parser.h"

typedef struct ArgsAndLastToken
{
    LinkedList* args;
    char* last_token;
} ArgsAndLastToken;

ArgsAndLastToken parse_args()
{
    ArgsAndLastToken argsAndLastToken;
    
    LinkedList* args = new_linked_list(NULL);

    char* token = strtok(NULL, " ");

    while (token != NULL && (strcmp(token, "&") != 0) && (strcmp(token, ">") != 0))
    {
        if (strcmp(token, "~") == 0)
            token = getenv("HOME");
            
        char* word = (char*) malloc(strlen(token) + 1);
        strcpy(word, token);

        add_linked_list_elem(args, word);

        token = strtok(NULL, " ");
    }

    argsAndLastToken.args = args;
    argsAndLastToken.last_token = token;

    return argsAndLastToken;
}

char* parse_redirection_filename(char* last_token)
{
    if ((last_token != NULL) && (strcmp(last_token, ">") == 0))
    {
        char* token = strtok(NULL, " ");
        char* filename = (char*) malloc(strlen(token) + 1);
        strcpy(filename, token);

        return filename;
    }
    else
    {
        return NULL;
    }
}

CommandGroup* parse_commands(char* user_input)
{
    LinkedList* commands = new_linked_list(NULL);

    char* token = strtok(user_input, " ");

    while(token != NULL)
    {
        ArgsAndLastToken argsAndLastToken = parse_args();

        char* redirection_filename = parse_redirection_filename(argsAndLastToken.last_token);

        LinkedList* args = argsAndLastToken.args;
        char* bin = (char*) malloc(strlen(token) + 1);
        strcpy(bin, token);

        ShellCommand* command = new_shell_command(bin, args, redirection_filename);
        free_linked_list(args, free);
        add_linked_list_elem(commands, command);

        token = strtok(NULL, " ");
    }

    CommandGroup* command_group = new_command_group(commands);
    return command_group;
}
