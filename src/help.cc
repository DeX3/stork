#include "help.h"

using namespace stork;

size_t help::terminal_columns() {

    struct winsize w;
    ioctl( 0, TIOCGWINSZ, &w );

    return w.ws_col;
}

std::vector<std::string> help::wordwrap( const std::string& str,
                            const std::string& prefix,
                            size_t max_columns ) {

    if( prefix.length() >= max_columns ) {
        throw ArgumentException( "Cannot wrap string to be "
                                 "smaller than the prefix" );
    }


    size_t w = prefix.length();

    boost::char_separator<char> sep( "\t\r\n " );
    boost::tokenizer< boost::char_separator<char> > words( str, sep );

    std::vector<std::string> lines;
    std::stringstream ss;

    ss << prefix;

    for( const auto& word : words ) {

        if( w + word.length() > max_columns ) {
            lines.push_back( ss.str() );
            ss.str( prefix );
            w = prefix.length();

            std::string current = word;
            if( w + current.length() > max_columns ) {
                while( w + current.length() > max_columns ) {

                    std::string w1 = current.substr( 0, max_columns - w );
                    std::string w2 = current.substr( max_columns - w );

                    ss << current.substr( 0, max_columns - w );
                    lines.push_back( ss.str() );
                    ss.str( prefix );

                    ss << current.substr( max_columns - w ) << " ";
                    w = prefix.length() + (current.length() - (max_columns + w)) + 1;
                }
            } else {
                ss << word << " ";
                w += word.length() + 1;
            }

        } else {
            ss << word << " ";
            w += word.length() + 1;
        }

    }

    lines.push_back( ss.str() );

    return lines;
}
