#include "stork.h"

#include <iostream>
#include <boost/program_options.hpp>

#include "template.h"
#include "utils.h"

using namespace stork;

namespace po = boost::program_options;

int cmd_new( int argc, char **argv );

int main( int argc, char **argv ) {


    if( argc < 2 ) {
        std::cout << "No command given." << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if( command == "new" ) {

        return cmd_new( argc - 1, argv + 1 );
    }else if( command == "help" ) {
        std::cout << "Available commands:" << std::endl;
        std::cout << "\tnew:\tCreate a new project" << std::endl;
        std::cout << "\thelp:\tDisplay this help text" << std::endl;
        std::cout << std::endl << "Use stork <command> help to display "<<
                     "command-specific help" << std::endl;
    }else {
        std::cout << "\"" << command <<
                     "\" is not a valid command" << std::endl;
        return 1;
    }
    
    return 0;
}

int cmd_new( int argc, char **argv ) {

    po::options_description desc( "Allowed options" );

    desc.add_options()
        ( "help", "Display help text" )
        ( "template", po::value<std::string>(), "Project template" )
        ( "name", po::value<std::string>() );

    po::positional_options_description p;
    p.add( "template", -1 );

    po::variables_map vm;
    po::store( po::command_line_parser( argc, argv ).
                options( desc ).positional( p ).run(), vm );
    po::notify( vm );

    if( vm.count( "template" ) ) {

        if( !vm.count( "name" ) ) {
            std::cout << "No project name specified!" << std::endl;
            return 1;
        }

        std::string template_name = vm["template"].as<std::string>();
        std::string project_name = vm["name"].as<std::string>();
        
        std::cout << "Creating new " << template_name << std::endl;

        Template tpl( TEMPLATE_LOCATION + template_name + ".dna" );
        std::cout << "Template created" << std::endl;

        auto read_param = []( const std::string& name ) {
            std::cout << name << "> ";

            std::string ret;
            std::getline( std::cin, ret );

            return ret;
        };

        ParamsStore params( read_param );
        params.set( "name", project_name );
        params.set( "template_name", template_name );


        tpl.instantiate( project_name, &params );
        
    }else {
        std::cout << "No template given." << std::endl;
    }

    return 0;
}
