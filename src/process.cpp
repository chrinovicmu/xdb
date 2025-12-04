#include <csignal>
#include <libxdb/process.hpp>
#include <memory>
#include <sched.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libxdb/error.hpp> 

std::unique_ptr<xdb::process> xdb::process::launch_proc(
    std::filesystem::path path){

    pid_t pid; 
    if((pid = fork()) < 0){
        xdb::error::send_errno("fork failed"); 
    }

    if(pid == 0){
        if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0){
            xdb::error::send_errno("Tracing failed"); 
        }
        if(execlp(path.c_str(), path.c_str(), nullptr) < 0){
            xdb::error::send_error("exec failed"); 
        }   
    }

    std::unique_ptr<process> proc (new process(pid, /*terminate_on_end*/true)); 
    proc->wait_on_signal(); 

    return proc; 

}

std::unique_ptr<xdb::process> xdb::process::attach_proc(pid_t pid){
    if(pid == 0){
        xdb::error::send("Invalid PID");
    }

    if(ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0){
        xdb::error::send("Could not attach process"); 
    }
    
    std::unique_ptr<process> proc (new process(pid, /*terminate_on_end*/ false)); 

    return proc; 
} 

xdb::process::~process(){

    if(_pid != 0){

        int status; 
        if(state == xdb::process_state::RUNNNING){
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

xdb::stop_reason::stop_reason(int wait_status){

    /*if process exited normally */ 
    if(WIFEXITED(wait_status)){
        reason = xdb::process_state::EXITED; 
        info = WEXITSTATUS(wait_status); 
    }

    /*if process terminated through signal */ 
    else if(WIFSIGNALED(wait_status)){
        reason = xdb::process_state::TERMINATED; 
        info = WTERMSIG(wait_status); 
    }

    /*if process stopped through signal */ 
    else if(WIFSTOPPED(wair_status)){
        reason = xdb::process_state::stopped; 
        info = WSTOPSIG(wait_status); 
    }

}

void xdb::process::resume(){

    /*PTRACE_CONT allows process to continue execution */ 
    if(ptrace(PTRACE_CONT, _pid, nullptrm, nullptr) < 0){
        xdb::error::send_error("Could not resume process"); 
    }

    _state = xdb::process_state::RUNNNING;   
}

xdb::stop_reason xdb::process::wait_on_signal(){
    
    int wait_status;
    int options = 0; 

    if(waitpid(_pid, &wait_status, options) < 0){
        xdb::error::send_error("waitpid failed"); 
    }

    stop_reason reason(wait_status); 
    _state = reason.reason; 

    return reason; 
}
