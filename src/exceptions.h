#ifndef STORK_EXCEPTIONS_H_INCLUDED
#define STORK_EXCEPTIONS_H_INCLUDED 

#include <sstream>
#include <string.h>
#include <stdarg.h>

namespace stork {
    /**
     * @brief Base class for all jsee_development_service related exceptions
     */
    class Exception : public std::exception {

    public:
        /**
         * @brief Returns the exception text.
         *
         * @return The exception text.
         */
        virtual const char * what() const throw() = 0;

        virtual ~Exception() throw() { }

    };

    class V8Exception : public Exception {

    public:
        V8Exception( const std::string& fmt, ... ) {

            va_list args1;
            va_list args2;

            va_start( args1, fmt );
            va_copy( args2, args1 );

            size_t needed = vsnprintf( NULL, 0, fmt.c_str(), args1 ) + 1;

            char *buf = (char *)malloc( needed );
            vsnprintf( buf, needed, fmt.c_str(), args2 );

            this->_msg = buf;
            
            free( buf );

            va_end( args1 );
            va_end( args2 );
        }

        ~V8Exception() throw() { }

        virtual const char * what() const throw() {
            return this->_msg.c_str();
        }

    protected:
        std::string _msg;       /**< The message of the exception */
    };


    class ArgumentException : public Exception {

    public:
        ArgumentException( const std::string& fmt, ... ) {

            va_list args1;
            va_list args2;

            va_start( args1, fmt );
            va_copy( args2, args1 );

            size_t needed = vsnprintf( NULL, 0, fmt.c_str(), args1 ) + 1;

            char *buf = (char *)malloc( needed );
            vsnprintf( buf, needed, fmt.c_str(), args2 );

            this->_msg = buf;
            
            free( buf );

            va_end( args1 );
            va_end( args2 );
        }

        ~ArgumentException() throw() { }

        virtual const char * what() const throw() {
            return this->_msg.c_str();
        }

    protected:
        std::string _msg;       /**< The message of the exception */
    };

}

#endif /* EXCEPTIONS_H_INCLUDED */
