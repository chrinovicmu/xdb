#pragma once 
#include <vector>
#include <cstddef>

namespace XDB {

class Pipe{

public:
    explicit Pipe(bool close_on_exec); 
    ~Pipe(); 

    int get_read() const {
        return _fds[read_fd]; 
    }
    int get_write() const {
        return _fds[write_fd]; 
    }

    int release_read();
    int release_write();
    void close_read();
    void close_write(); 

    std::vector<std::byte> read();
    void write(std::byte* from, std::size_t bytes); 

private:
    static constexpr unsigned read_fd = 0; 
    static constexpr unsigned write_fd = 1; 

    int _fds[2]; 

}; 

}
