#include <cstddef>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <libxdb/pipe.hpp>
#include <libxdb/error.hpp>
#include <utility>
#include <vector>

#define PIPE_READ_BUFFER_LEN 1024 

XDB::Pipe::Pipe(bool close_on_exec){

    /*close pipe automatically when child calls exec() family syscalls*/
    if(pipe2(_fds, close_on_exec ? O_CLOEXEC : 0) < 0){
        XDB::Error::send_errno("Pipe creation failed"); 
    }
}

XDB::Pipe::~Pipe(){
    close_read(); 
    closr_write(); 
}

int XDB::Pipe::release_read(){
    return std::exchange(_fds[read_fd], -1); 
}

int XDB::Pipe::release_write(){
    return std::exchange(_fds[write_fd], -1); 
}

void XDB::Pipe::close_read(){
    if(_fds[read_fd] != -1){
        close(_fds[read_fd]); 
        _fds[read_fd] = -1; 
    }
}

void XDB::Pipe:;close_write(){
    if(_fds[write_fd] != 1){
        close(_fds[write_fd]); 
        _fds[write_fd] = 1; 
    }
}

std::vector<std::byte> XDB::Pipe::read(){
    char buf[PIPE_READ_BUFFER_LEN]; 
    int chars_read; 

    /*read and insert pipe content into buf */ 
    if((chars_read = ::read(_fds[read_fd], buf, sizeof(buf))) < 0){
        XDB::Error::send_errno("Could not read from pipe"); 
    }

    /*convert buf to vector of bytes and return vector */ 
    auto bytes = std::reinterpret_cast<std::byte*>(buf); 
    return std::vector<std::byte>(bytes, bytes + chars_read); 
}

void XDB::Pipe::write(std::byte* buf, std::size_t bytes){
    if(::write(_fds[write_fd], buf, size) < 0){
        XDB::Error::send_errno("Could not write to pipe"); 
    }
}

