#include <csignal>
#include <memory>
#include <sched.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libxdb/process.hpp> 
#include <libxdb/error.hpp> 

/*static method that launches a new process to be debugged */ 
std::unique_ptr<XDB::Process> XDB::Process::launch_proc(
    std::filesystem::path path){

    pid_t pid; 
    if((pid = fork()) < 0){
        XDB::Error::send_errno("fork failed"); 
    }


    if(pid == 0){

        /*allow tracing by parent process */ 
        if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0){
            XDB::Error::send_errno("Tracing failed"); 
        }

        /*replace child proces image with process at $PATH*/
        if(execlp(path.c_str(), path.c_str(), nullptr) < 0){
            XDB::Error::send_errno("exec failed"); 
        }   
    }

    std::unique_ptr<Process> proc (new Process(pid, /*terminate_on_end*/true)); 
    proc->wait_on_signal(); 

    return proc; 

}

/*attach debugger to already running process */ 
std::unique_ptr<XDB::Process> XDB::Process::attach_proc(pid_t pid){

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
