#ifndef STORK_UTILS_H_INCLUDED
#define STORK_UTILS_H_INCLUDED


#include <string>
#include <functional>
#include <algorithm>

namespace stork {

    namespace utils {

        void walk( const std::string& path,
                          std::function<void(std::string)> fn );


        std::string read_file_contents( const std::string& filename );

        
        // trim from start
        static inline std::string &ltrim(std::string &s) {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                            std::not1(std::ptr_fun<int, int>(std::isspace))));
                return s;
        }

        // trim from end
        static inline std::string &rtrim(std::string &s) {
                s.erase(std::find_if(s.rbegin(), s.rend(),
                            std::not1(
                                std::ptr_fun<int, int>(std::isspace))).base(),
                                s.end());
                return s;
        }

        // trim from both ends
        static inline std::string &trim(std::string &s) {
                return ltrim(rtrim(s));
        }

        
        void replace( std::string& str,
                      const std::string& from,
                      const std::string& to);

        std::string get_home();

        std::string fullpath( const std::string& path );


    }
}
#endif /* STORK_UTILS_H_INCLUDED */
