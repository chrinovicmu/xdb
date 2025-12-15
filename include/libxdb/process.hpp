#pragma once 

#include "registers.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <sys/types.h> 
#include <libxdb/registers.hpp> 
#include <sys/user.h>

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

    Registers& get_registers(){
        return *_registers;
    }
    const Registers& get_registers() const{
        return *_registers; 
    }

    void write_user_area(std::size_t offset, std::uint64_t data);

    void write_fprs(const user_fpregs_struct& fprs); 
    void write_gprs(const user_regs_struct& gprs); 

private:
    pid_t _pid = 0;    
    bool _terminate_on_end = true;
    bool is_attached = true; 
    std::unique_ptr<Registers> _registers; 

    ProcessState _state = ProcessState::STOPPED;
    Process(pid_t pid, bool terminate_on_end, bool is_attached): 
        _pid(pid),
        _terminate_on_end(terminate_on_end),
        is_attched(is_attached), _registers(new Registers(this*))
   
    {}
    void read_all_registers(); 

}; 


} /*namespace XDB */  




