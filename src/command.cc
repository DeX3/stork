#include "command.h"

std::map<std::string, Command> Command::commands = Command::create_commands();
