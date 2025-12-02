#pragma once 

#include <stdexcept>
#include <cstring>
#include <string>

namespace xdb {

class error: public std::runtime_error{

public:
    [[noreturn]] 
    static void send(const std::string& err){
        throw error(err); 
    }

    [[noreturn]]
    static void send_errno(const std::string& prefix){
        throw_error(prefix + ": " + std::strerror(errno)); 
    }
private:
    error(const std::string& err) : std::runtime_error(err) {}
}; 

}
