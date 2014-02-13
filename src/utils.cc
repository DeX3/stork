#include "utils.h"

#include <fstream>
#include <string>
#include <cerrno>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <linux/limits.h>

using namespace stork;
using namespace boost;

void utils::walk( const std::string& path,
                  std::function<void(std::string)> fn ) {

    filesystem::path current( path );

    for( filesystem::recursive_directory_iterator iter( current ), end;
         iter != end;
         ++iter ) {

        std::string name = iter->path().string();
        fn( name );
    }

    std::cout << "walk done in utils" << std::endl;

}

std::string utils::read_file_contents( const std::string& filename ) {

    std::ifstream in(filename, std::ios::in | std::ios::binary);

    if( in ) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();

        return contents;
    }

    throw(errno);
}

void utils::replace( std::string& str,
                     const std::string& from,
                     const std::string& to) {

    if( from.empty() ) {
        return;
    }


    size_t start_pos = 0;
    while( (start_pos = str.find(from, start_pos)) != std::string::npos ) {
        str.replace(start_pos, from.length(), to);
        // In case 'to' contains 'from', like replacing 'x' with 'yx'
        start_pos += to.length(); 
    }
}

std::string utils::get_home() {

    struct passwd *pw = getpwuid( getuid() );

    return std::string( pw->pw_dir );
}

std::string utils::fullpath( const std::string& path ) {
    
    char actualpath[PATH_MAX];

    char *ptr = realpath( path.c_str(), actualpath );

    return std::string( ptr );
}

