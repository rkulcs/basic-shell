#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#include "linked_list.h"
#include "command_paths.h"

#define MAX_WORD_LENGTH 1024

typedef struct ShellCommand
{
    char* bin;
    int num_args;
    char** args;
    char* redirection_filename;
} ShellCommand;

typedef struct CommandGroup
{
    int num_commands;
    ShellCommand** commands;
} CommandGroup;

typedef struct CommandAndPathGroup
{
    ShellCommand* command;
    CommandPaths* paths;
} CommandAndPathGroup;

CommandGroup* new_command_group(LinkedList* commands);
ShellCommand* new_shell_command(char* bin, LinkedList* args, char* redirection_filename);
int execute_commands(CommandGroup* group, CommandPaths* command_paths);
void free_command_group(CommandGroup* group);
void free_shell_command(ShellCommand* shell_command);
