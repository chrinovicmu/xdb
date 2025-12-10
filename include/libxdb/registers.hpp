#pragma once 

#include "types.hpp"
#include <cstdint>
#include <sys/user.h>
#include <libxdb/register_info.hpp>

namespace XDB {

class Process; 

class Registers{
public:

    Registers() = delete;
    Registers(const Registers&) = delete; 
    Registers& operator = (const Registers&) = delete; 

    using value = std::variant<
        std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, 
        std::int8_t, std::int16_t, std::int32_t, std::int64_t, 
        float, double, long double,
        byte64, byte128>; 

    value read(const RegisterInfo& info) const; 
    void write(const RegisterInfo& info, value value);

    template<class T>
    T read_by_id_as(RegisterID id) const{
        return std::get<T>(read(register_info_by_id(id))); 
    }

    void write_by_id(RegisterID id, value val){
        write(register_info_by(id), val); 
    }

private:
    friend Process;
    Registers(Process& proc) : _proc(&proc) {}

    user _data; 
    Process * _proc; 
}; 

}
