#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libxdb/libxdb.hpp> 
#include <iostream>
#include <memory>
#include <sched.h>
#include <sstream>
#include <string_view>
#include <unistd.h>
#include <editline/readline.h>
#include <string>
#include <vector>
#include <libxdb/process.hpp>

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

std::unique_ptr<xdb::process> attach(int argc, const char **argv){

    
    if(argc == 3 && argv[1] == std::string_view("-p")){
        
        pid_t pid = std::atoi(argv[2]); 
        return xdb::process::attach_proc(pid); 
    }

    else{
        const char * program_path = argv[1]; 
        return xdb::process::launch_proc(program_path);  
    }
 
}
    
void print_stop_reason(const xdb::process& process, xdb::stop_reason reason){

    std::cout << "pid " << "process.pid()" << ' '; 

    switch (reason.reason){

        case xdb::process_state::EXITED:
            std::cout << "exited with status " 
                << static_cast<int>(reason.info);
            break; 

        case xdb::process_state::TERMINATED:
            std::cout << "terminated with signal "
                << sigabbrev_np(reason.info);
            break; 

        case xdb::process_state::STOPPED:
            std::cout << "stopped with signal "
                << sigabbrev_np(reason.info); 
            break; 

    }

    std::cout << std::endl; 
}

void handle_command(std::unique_ptr<xdb::process>& process, 
                    std::string_view){
    
    auto args = split(line, ' '); 
    auto command = args[0]; 
    
    if(is_prefix(command, "coninue")){
        process->resume(); 
        auto reason = process->wait_on_signal(); 
        print_stop_reason(*process, reason)

    }
    else{
        std::cerr  << "Unknown command\n"; 
    }
}

void main_loop(std::unique_ptr<xdb::process>& process){
    
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
            try {
                handle_command(process, line_str); 
            }
            catch(const xdb::error& err){
                std::cout << err.err() << '\n'; 
            }
        }
    }
}
}

int main (int argc, char **argv) {

    if(argc == 1){
        std::cerr << "No arguments given\n"; 
        return -1; 
    }

    try{
        auto process = attach(argc, **argv); 
        main_loop(process); 
    }
    catch (const xdb::error& err){
        std::cout << err.err() << '\n';
    }
}

