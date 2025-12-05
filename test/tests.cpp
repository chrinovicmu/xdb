#include <catch2/catch_test_macros.hpp> 
#include <cerrno>
#include <signal.h>
#include <libxdb/process.hpp>

namespace  {

bool process_exits(pid_t pid){
    auto ret = kill(pid, 0); 
    return ret != -1 and errno != ESRCH; 
}

}

TEST_CASE("xdb::process::launch sucess", "[process]"){
    auto proc = XDB::Process::launch_proc("yes"); 
    REQUIRE(process_exits(proc->pid())); 
}
