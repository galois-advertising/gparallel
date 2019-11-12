#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cxxabi.h>

namespace galois::gparallel 
{

std::string demangle(const char* name) {
    int status = -4;
    auto origin_name = std::string(name);
    auto demangle_name = abi::__cxa_demangle(name, NULL, NULL, &status);
    return (status == 0) ? std::string(demangle_name) : origin_name;
};
}