#include <catch2/catch_test_macros.hpp> 
#include <cerrno>
#include <fstream>
#include <sched.h>
#include <signal.h>
#include <libxdb/process.hpp>
#include <libxdb/error.hpp>

namespace  {

bool process_exits(pid_t pid){
    auto ret = kill(pid, 0); 
    return ret != -1 and errno != ESRCH; 
}

char get_process_status(pid_t pid){
    std::ifstream stat("/proc/" + std::to_string(pid) + "/stat"); 
    std::string data; 
    std::getline(stat, data);

    auto index_of_last_parenthesis = data.rfind(')'); 
    auto inde_of_status_indicator = index_of_last_parenthesis + 2;
    
    return data[inde_of_status_indicator]; 
}

}

TEST_CASE("XDB::Process::launch_proc sucess", "[Process]"){
    auto proc = XDB::Process::launch_proc("yes"); 
    REQUIRE(process_exits(proc->pid())); 
}

TEST_CASE("XDB::Process::launch_proc no program", "[Process]"){
    REQUIRE_THROWS_AS(XDB::Process::launch_proc("you_do_not_have_to_be_that_good"), XDB::Error); 
}

TEST_CASE("XDB::Process::attach_proc success", "[Process]"){
    auto target = XDB::launch("targets/run_endlessly", false);
    auto proc = XDB::Process::attach_proc(target->pid()); 
    REQUIRE(get_proces_status(target->pid()) == 't'); 
}

