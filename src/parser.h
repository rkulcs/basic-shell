#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "shell_command.h"
#include "linked_list.h"

CommandGroup* parse_commands(char* user_input);
