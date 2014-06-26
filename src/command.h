#ifndef STORK_COMMAND_H_INCLUDED
#define STORK_COMMAND_H_INCLUDED

#include <vector>

namespace stork {

class Command {

public:
    std::string name() const {
        return this->name_;
    }
    std::string short_description() const {
        return this->short_description_;
    }
    std::string description() const {
        return this->description_;
    }

    static std::map<std::string,Command> commands;

    Command() {
    }

private:


    Command( std::string name,
             std::string short_description,
             std::string description )
        : name_( name ),
          short_description_( short_description ),
          description_( description ) {

    }

    std::string name_;
    std::string short_description_;
    std::string description_;

    static std::map<std::string,Command> create_commands() {

        std::map<std::string, Command> ret;
        ret["help"] = Command(
                            "help",
                            "Display this help text",
                            "Display a help text explaining how to use stork" );

        ret["new"] = Command(
                            "new",
                            "Create a new project",
                            "TODO" );

        return ret;
    }



};

}


#endif /* STORK_COMMAND_H_INCLUDED */
