#pragma once 

#include <cstdint>
#include <filesystem>
#include <memory>
#include <sys/types.h> 

namespace XDB {

enum class ProcessState{
    STOPPED, 
    RUNNNING,
    EXITED,
    TERMINATED 
}; 

struct StopReason{
    StopReason(int wait_status); 
    ProcessState reason; 
    std::uint8_t info; 
}; 

class Process{

public:

    Process() = delete; 
    Process(const Process&) = delete; 
    Process& operator = (const Process&) = delete; 

    static std::unique_ptr<Process> launch_proc(
        std::filesystem::path path, bool debug = true); 

    static std::unique_ptr<Process> attach_proc(pid_t pid); 
        
    void resume(); 
    struct StopReason wait_on_signal(); 

    pid_t pid() const {
        return _pid;
    }

    ProcessState state() const {
        return _state; 
    }
    ~Process(); 


private:
    pid_t _pid = 0;    
    bool _terminate_on_end = true;
    bool is_attached = true; 

    ProcessState _state = ProcessState::STOPPED;
    Process(pid_t pid, bool terminate_on_end, bool is_attached): 
        _pid(pid),
        _terminate_on_end(terminate_on_end),
        is_attched(is_attached)
   
    {}
}; 


} /*namespace XDB */  




