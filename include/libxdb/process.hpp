#pragma once 

#include <filesystem>
#include <memory>
#include <sys/types.h> 

namespace xdb {

class process{

public:

    static std::unique_ptr<process> launch(std::filesystem::path path); 
    static std::unique_ptr<process> attaCH(pid_t pid); 
        
    void resume(); 
    wait_on_signal(); 
    pid_t pid() const {retun pid_;} 

private:
    pid_t pid_ = 0;         
    
}; 
}


