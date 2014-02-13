#ifndef STORK_PARAMS_STORE_H_INCLUDED
#define STORK_PARAMS_STORE_H_INCLUDED

#include <string>
#include <map>
#include <functional>

namespace stork {

    typedef std::function<
                            std::string( const std::string& name )
                         > ReadParamCallback;

    class ParamsStore {

    public:
        ParamsStore( ReadParamCallback read_param )
            : read_param_( read_param ) {
        }

        std::string get( const std::string& str );
        void set( const std::string& name, const std::string& value );

    private:
        std::map<std::string, std::string> params_;
        ReadParamCallback read_param_;
    };
}

#endif /* STORK_PARAMS_STORE_H_INCLUDED */
