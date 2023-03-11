#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "interface.h"
#include "command_paths.h"
#include "shell_command.h"
#include "parser.h"

int main(int argc, char* argv[])
{
    CommandPaths* command_paths = init_command_paths();

    int exit_requested = 0;

    while (exit_requested != 1)
    {
        char* user_input = get_command();
        CommandGroup* commands = parse_commands(user_input);
        free(user_input);

        exit_requested = execute_commands(commands, command_paths);

        free_command_group(commands);
    }

    free_command_paths(command_paths);
}
