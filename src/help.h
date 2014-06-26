#ifndef STORK_HELP_H_INCLUDED
#define STORK_HELP_H_INCLUDED

#include <sys/ioctl.h>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include "exceptions.h"


namespace stork {
namespace help {

    size_t terminal_columns();

    std::vector<std::string> wordwrap( const std::string& str,
                                       const std::string& prefix,
                                       size_t max_columns );
}
}


#endif /* STORK_HELP_H_INCLUDED */
