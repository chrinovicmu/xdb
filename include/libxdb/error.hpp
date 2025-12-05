#pragma once 

#include <stdexcept>
#include <cstring>
#include <string>

namespace XDB {

class Error: public std::runtime_error{

public:
    [[noreturn]] 
    static void send(const std::string& err){
        throw Error(err); 
    }

    [[noreturn]]
    static void send_errno(const std::string& prefix){
        throw Error(prefix + ": " + std::strerror(errno)); 
    }
private:
    Error(const std::string& err) : std::runtime_error(err) {}
}; 

}
