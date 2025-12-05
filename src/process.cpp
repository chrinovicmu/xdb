#include <csignal>
#include <memory>
#include <sched.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libxdb/process.hpp> 
#include <libxdb/pipe.hpp> 
#include <libxdb/error.hpp> 

namespace{

void exit_with_perror(XDB::Pipe& channel,
                      std::string const& prefix){
    auto message = prefix + ": " + std::strerror(errno); 

    channel.write(reinterpret_cast<std::byte*>(message.data()), message.size()); 
    exit(-1); 
}

}
/*static method that launches a new process to be debugged */ 
std::unique_ptr<XDB::Process> XDB::Process::launch_proc(
    std::filesystem::path path){

    bool terminate_on_end = true; 

    pid_t pid; 
    if((pid = fork()) < 0){
        XDB::Error::send_errno("fork failed"); 
    }


    if(pid == 0){
        
        channel.close_read(); 
        /*allow tracing by parent process */ 
        if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0){
            exit_with_perror(channel, "Tracing failed")
        }

        /*replace child proces image with process at $PATH*/
        if(execlp(path.c_str(), path.c_str(), nullptr) < 0){
            exit_with_perror(channel, "exec failed"); 
        }   
    }

    /*collect pipe content*/ 
    channel.close_write(); 
    auto data = channel.read(); 
    channel.close_read(); 

    /*if channel has data, throw an execption */ 
    if(data.size() > 0){
        waitpid(pid, nullptr, 0); 
        auto chars s= reinterpret_cast(char*)(data.data()); 
        XDB::Error::send(std::string(chars, char + data.size()));
    }

    std::unique_ptr<Process> proc (new Process(pid, terminate_on_end)); 
    proc->wait_on_signal(); 

    return proc; 

}

/*attach debugger to already running process */ 
std::unique_ptr<XDB::Process> XDB::Process::attach_proc(pid_t pid){

    bool close_on_exec = true; 

    XDB::Pipe channel(close_on_exec); 

    if(pid == 0){
        XDB::Error::send("Invalid PID");
    }

    if(ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0){
        XDB::Error::send("Could not attach process"); 
    }
    
    std::unique_ptr<Process> proc (new Process(pid, /*terminate_on_end*/ false)); 

    return proc; 
} 

XDB::Process::~Process(){

    if(_pid != 0){

        int status; 
        if(state() == ProcessState::RUNNNING){
            kill(_pid, SIGSTOP); 
            waitpid(_pid, &status, 0); 
        }

        ptrace(PTRACE_DETACH, _pid, nullptr, nullptr); 
        kill(_pid, SIGCONT);

        if(_terminate_on_end){
            kill(_pid, SIGKILL);
            waitpid(_pid, &status, 0); 
        }

    }
}

XDB::StopReason::StopReason(int wait_status){

    /*if process exited normally */ 
    if(WIFEXITED(wait_status)){
        reason = XDB::ProcessState::EXITED; 
        info = WEXITSTATUS(wait_status); 
    }

    /*if process terminated through signal */ 
    else if(WIFSIGNALED(wait_status)){
        reason = XDB::ProcessState::TERMINATED; 
        info = WTERMSIG(wait_status); 
    }

    /*if process stopped through signal */ 
    else if(WIFSTOPPED(wait_status)){
        reason = XDB::ProcessState::STOPPED; 
        info = WSTOPSIG(wait_status); 
    }

}

void XDB::Process::resume(){

    /*PTRACE_CONT allows process to continue execution */ 
    if(ptrace(PTRACE_CONT, _pid, nullptr, nullptr) < 0){
        XDB::Error::send_errno("Could not resume process"); 
    }

    _state = XDB::ProcessState::RUNNNING;   
}

XDB::StopReason XDB::Process::wait_on_signal(){
    
    int wait_status;
    int options = 0; 

    if(waitpid(_pid, &wait_status, options) < 0){
        XDB::Error::send_errno("waitpid failed"); 
    }

    StopReason reason(wait_status); 
    _state = reason.reason; 

    return reason; 
}
