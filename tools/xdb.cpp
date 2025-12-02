#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <libxdb/libxdb.hpp> 
#include <iostream>
#include <sched.h>
#include <sstream>
#include <string_view>
#include <unistd.h>
#include <editline/readline.h>
#include <string>
#include <vector>

namespace {

    std::vector<std::string> spit(std::string_view str, char delimiter){
    
        std::vector<std::string> ret{}; 
        std::std::stringstream ss {std::string{str}}; 
        std::string item; 

        while(std::getline(ss, item, delimiter)){
            out.pushback(item); 
        }
        return ret; 
    }   

    bool is_prefix(std::string_view str, std::string_view of){
        
        if(str.size() > of.size())
            return false; 

        return std::equal(str.begin(), str.end(), of.begin()); 
    }

    void resume(pid_t pid){

        if(ptrace(PTRACE_CONT, pid, nullptr, nullptr) < 0){
            std::cerr << "Couldn't continue\n ";
            std::exit(-1); 
        }       
    }

    void wait_on_signal(pid_t pid){
        
        int wait_status;
        int options = 0; 

        if(waitpid(pid, &wait_status, options) < 0){
            std::perror("waitpid failed");
            std::exit(-1); 
        }
        
    }
    pid_t attach(int argc, const char **argv){

        pid_t pid = 0; 
        
        if(argc == 3 && argv[1] == std::string_view("-p")){
            pid = std::atoi(argv[2]); 
            if(pid <= 0){
                std::cerr "Invalid pid\n"; 
                return -1; 
            }

            if(ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0){
                std::perror("Could not attach pid"); 
                return -1; 
            }    
        }
        else{
            if((pid = fork()) < 0){
                std::perror("fork failed"); 
                return -1; 
            }
            
            if(pid == 0){
                /*child process */ 
                
                if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0){
                    std::perror("Tracing failed");
                    return -1; 
                }
            }
        } 
        
    }

    void handle_command(pid_t pid, std::string_view line){
        
        auto args = split(line, ' '); 
        auto command = args[0]; 
        
        if(is_prefix(command, "coninue")){
            resume(pid); 
            wait_on_signal(pid); 

        }
        else{
            std::cerr  << "Unknown command\n"; 
    }
        
    }

}
int main (int argc, char **argv) {

    if(argc == 1){
        std::cerr << "No arguments given\n"; 
        return -1; 
    }

    pid_t pid = attach(argc, argv);

    int wait_status; 
    int options = 0; 

    if(waitpid(pid, &wait_status, options) < 0){
        std::perror("waitpid failed"); 

    }

    char *line = nullptr; 
    
    while((line = readline("xdb> ")) != nullptr){

        /*holds commad to be executed */ 
        std::string line_str; 

        /*if line empty: retrieve last recently read line */ 
        if(line == std::string_view("")){
            free(line); 
            if(history_length > 0){
                line_str = history_list()[history_length -1]->line; 
            }
        }
        else{
            line_str = line; 
            add_history(line); 
            free(line); 
        }

        if(!line_str.empty()){
            handle_command(pid, line_str); 
        }
    }
    return 0;
}

