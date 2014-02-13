#include "params_store.h"

using namespace stork;

std::string ParamsStore::get( const std::string& str ) {

    const auto& i_name = params_.find( str );

    std::string value;
    if( i_name == params_.end() ) {
        value = params_[str] = read_param_( str );
    }else {
        value = params_[str];
    }

    return value;
}

void ParamsStore::set( const std::string& name,
                       const std::string& value ) {

    params_[name] = value;
}
