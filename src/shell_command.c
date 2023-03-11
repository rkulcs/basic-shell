#include "shell_command.h"

CommandGroup* new_command_group(LinkedList* commands)
{
    CommandGroup* group = (CommandGroup*) malloc(sizeof(CommandGroup));
    group->num_commands = commands->num_elems;

    if (commands->num_elems == 0)
    {
        group->commands = NULL;
    }
    else
    {
        group->commands = (ShellCommand**) malloc((commands->num_elems) * sizeof(ShellCommand*));

        int i = 0;

        for (Node* node = commands->first; node != NULL; node = node->next)
        {
            group->commands[i] = (ShellCommand*) node->elem;
            i++;
        }
    }

    free_linked_list(commands, NULL);

    return group;
}

ShellCommand* new_shell_command(char* bin, LinkedList* args, char* redirection_filename)
{
    ShellCommand* command = (ShellCommand*) malloc(sizeof(ShellCommand));
    command->bin = bin;
    command->num_args = args->num_elems;
    command->redirection_filename = redirection_filename;

    if (args->num_elems == 0)
    {
        command->args = NULL;
    }
    else
    {
        command->args = (char**) malloc(((command->num_args) + 2) * sizeof(char*));

        // Set the command to be the first argument
        command->args[0] = command->bin;

        // Create array of strings from linked list of arguments
        int i = 1;

        for (Node* node = args->first; node != NULL; node = node->next)
        {
            command->args[i] = malloc(strlen(node->elem) + 1);
            strcpy(command->args[i], node->elem);
            i++;
        }

        command->args[i] = NULL;
    }

    return command;
}

bool is_builtin_command(char* command_name)
{
    return ((strcmp(command_name, "exit\0") == 0) 
        || (strcmp(command_name, "cd\0") == 0)
        || (strcmp(command_name, "path\0") == 0));
}

void set_full_bin_path(ShellCommand* shell_command, CommandPaths* command_paths)
{
    if (is_builtin_command(shell_command->bin))
        return;

    char* path = NULL;

    for (int i = 0; i < command_paths->num_paths; i++)
    {
        path = command_paths->paths[i];

        // Concatenate the path and command to obtain the full path to the command
        char* full_bin_path = (char*) malloc(strlen(path) + strlen(shell_command->bin) + 1);
        strcpy(full_bin_path, path);
        strcat(full_bin_path, shell_command->bin);

        if (access(full_bin_path, X_OK) == 0)
        {
            // Update the "bin" field of the command to store the full path to the executable
            free(shell_command->bin);
            shell_command->bin = full_bin_path;

            if (shell_command->args != NULL)
                shell_command->args[0] = full_bin_path;

            break;
        }

        free(full_bin_path);
    }
}

bool is_valid_command(ShellCommand* shell_command)
{
    if (is_builtin_command(shell_command->bin))
    {
        if (strcmp(shell_command->bin, "exit\0") == 0)
        {
            if (shell_command->args != NULL)
            {
                printf("exit does not take any arguments.\n");
                return false;
            }
            else
            {
                return true;
            }
        }
        else if (strcmp(shell_command->bin, "cd\0") == 0)
        {
            if (shell_command->num_args != 1)
            {
                printf("cd takes a single argument.\n");
                return false;
            }
            else
            {
                return true;
            }
        }

        return true;
    }
    else
    {
        bool is_valid = (access(shell_command->bin, X_OK) == 0);

        if (!is_valid)
            printf("%s: Command not found.\n", shell_command->bin);

        return is_valid;
    }
}

int change_dir(char* path)
{
    if (strcmp(path, "~\0") == 0)
        return chdir(getenv("HOME"));
    else
        return chdir(path);
}

void* execute_command(void* args)
{
    CommandAndPathGroup* command_and_paths = (CommandAndPathGroup*) args;
    ShellCommand* shell_command = command_and_paths->command;
    CommandPaths* command_paths = command_and_paths->paths;

    if (shell_command->bin == NULL)
        return (void*) -1;

    set_full_bin_path(shell_command, command_paths);
    
    if (!is_valid_command(shell_command))
        return (void*) -1;

    if (strcmp(shell_command->bin, "exit\0") == 0)
        return (void*) 1;

    if (strcmp(shell_command->bin, "cd\0") == 0)
    {
        return (void*) change_dir(shell_command->args[1]);
    }

    if (strcmp(shell_command->bin, "path\0") == 0)
    {
        return (void*) update_command_paths(command_paths, shell_command->num_args, shell_command->args);
    }

    int rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "Failed to execute command.\n");
        exit(1);
    }
    else if (rc == 0)
    {
        if (shell_command->redirection_filename != NULL)
        {
            int file_out = open(shell_command->redirection_filename, O_RDWR|O_CREAT|O_APPEND, 0600);
            int stdout_backup = dup(fileno(stdout));
            dup2(file_out, fileno(stdout));

            execv(shell_command->bin, shell_command->args);

            fflush(stdout);
            close(file_out);
            dup2(stdout_backup, fileno(stdout));
            close(stdout_backup);
        }
        else
        {
            execv(shell_command->bin, shell_command->args);
        }
    }
    else
    {
        // Wait for the command to finish execution
        int rc_wait = wait(NULL);
    }

    return (void*) 0;
}

int execute_commands(CommandGroup* group, CommandPaths* command_paths)
{
    pthread_t** threads = (pthread_t**) malloc(sizeof(pthread_t*) * group->num_commands);
    CommandAndPathGroup** group_data = (CommandAndPathGroup**) malloc(
        sizeof(CommandAndPathGroup*) * group->num_commands);

    int status = 0;

    // Create and execute a thread for each command
    for (int i = 0; i < group->num_commands; i++)
    {
        threads[i] = malloc(sizeof(pthread_t));
        group_data[i] = malloc(sizeof(CommandAndPathGroup));
        group_data[i]->command = group->commands[i];
        group_data[i]->paths = command_paths;

        pthread_create(threads[i], NULL, execute_command, group_data[i]);
    }

    // Wait for the commands to finish execution
    for (int i = 0; i < group->num_commands; i++)
    {
        void* void_return_value;
        pthread_join(*threads[i], &void_return_value);

        if (status != 1)
            status = (int) void_return_value;
    }

    for (int i = 0; i < group->num_commands; i++)
    {
        free(threads[i]);
        free(group_data[i]);
    }

    free(threads);
    free(group_data);

    return status;
}

void free_command_group(CommandGroup* group)
{
    for (int i = 0; i < group->num_commands; i++)
    {
        free_shell_command(group->commands[i]);
    }

    free(group->commands);
    free(group);
}

void free_shell_command(ShellCommand* shell_command)
{
    free(shell_command->bin);

    if (shell_command->args != NULL)
    {
        for (int i = 1; i < (shell_command->num_args + 2); i++)
            free(shell_command->args[i]);

        free(shell_command->args);
    }

    if (shell_command->redirection_filename != NULL)
        free(shell_command->redirection_filename);

    free(shell_command);
}
