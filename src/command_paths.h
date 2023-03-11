#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct CommandPaths
{
    int num_paths;
    char** paths;
} CommandPaths;

CommandPaths* init_command_paths();
int update_command_paths(CommandPaths* path_group, int num_paths, char** path_cmd_args);
void free_command_paths(CommandPaths* path_group);
