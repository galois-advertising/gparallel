#include <map>
#include <vector>
#include <string>
#include <memory>
#include <boost/log/trivial.hpp>
#include "util.h"

namespace galois::gparallel 
{



#ifdef __GNUG__
#include <cstdlib>
#include <cxxabi.h>
// see https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
std::string demangle(const char* name) {
    int status = -4; // some arbitrary value to eliminate the compiler warning
    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };
    return (status==0) ? res.get() : name ;
}
#else
// does nothing if not g++
std::string demangle(const char* name) {
    return name;
}
#endif

void log(LOG_LEVEL loglevel, const char * fmt, ...)
{
    va_list args;
    char buf[1024];
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    switch(loglevel) {
    case TRACE:BOOST_LOG_TRIVIAL(trace) << buf;break;
    case DEBUG:BOOST_LOG_TRIVIAL(debug) << buf;break;
    case INFO:BOOST_LOG_TRIVIAL(info) << buf;break;
    case WARNING:BOOST_LOG_TRIVIAL(warning) << buf;break;
    case ERROR:BOOST_LOG_TRIVIAL(error) << buf;break;
    case FATAL:BOOST_LOG_TRIVIAL(fatal) << buf;break;
    };
}
}