#include "command_paths.h"

CommandPaths* init_command_paths()
{
    CommandPaths* path_group = malloc(sizeof(CommandPaths));
    path_group->num_paths = 1;
    path_group->paths = malloc(sizeof(char*));
    path_group->paths[0] = malloc(strlen("/bin/") + 1);
    strcpy(path_group->paths[0], "/bin/");
    return path_group;
}

void free_path_array(CommandPaths* path_group)
{
    if (path_group->paths != NULL)
    {
        for (int i = 0; i < path_group->num_paths; i++)
            free(path_group->paths[i]);

        free(path_group->paths);
    }
}

int update_command_paths(CommandPaths* path_group, int num_paths, char** path_cmd_args)
{
    free_path_array(path_group);
    path_group->num_paths = num_paths;

    if (num_paths == 0)
        return 0;

    path_group->paths = malloc(sizeof(char*) * num_paths);

    for (int i = 0; i < num_paths; i++)
    {
        // Check if a trailing forward slash needs to be appended to the path
        bool add_trailing_slash = (path_cmd_args[i + 1][strlen(path_cmd_args[i + 1]) - 1] != '/');

        // Ignore the "path" and "NULL" parts of the command arguments
        path_group->paths[i] = malloc(strlen(path_cmd_args[i + 1])
                + (add_trailing_slash ? 2 : 1));
        strcpy(path_group->paths[i], path_cmd_args[i + 1]);

        if (add_trailing_slash)
            strcat(path_group->paths[i], "/");
    }

    return 0;
}

void free_command_paths(CommandPaths* path_group)
{
    free_path_array(path_group);
    free(path_group);
}
