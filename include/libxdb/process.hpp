#pragma once 

#include <cstdint>
#include <filesystem>
#include <memory>
#include <sys/types.h> 

namespace xdb {

enum class process_state{
    STOPPED, 
    RUNNNING,
    EXITED,
    TERMINATED 
}; 

struct stop_reason{
    stop_reason(int wait_status); 
    process_state reason; 
    std::uint8_t info; 
}; 

class process{

public:

    process() = delete; 
    process(const process&) = delete; 
    process& operator = (const process&) delete; 

    static std::unique_ptr<process> launch_proc(std::filesystem::path path); 
    static std::unique_ptr<process> attach_proc(pid_t pid); 
        
    void resume(); 
    struct stop_reason wait_on_signal(); 

    pid_t pid() const {
        return _pid;
    }

    process_state state() const {
        return _state; 
    }
    ~process(); 


private:
    pid_t _pid = 0;    
    bool _terminate_on_end = true; 
    process_state _state = process_state::STOPPED;
    process(pid_t pid, bool terminate_on_end) : pid_(pid), _terminate_on_end((terminate_on_end)){}
}; 


}




