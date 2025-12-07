#pragma once 

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <sys/user.h>


namespace XDB{

enum class RegisterID{

}; 

enum class RegisterType{
    GPR,
    SUBGPR, 
    FPR, 
    DR
}; 

enum class RegisterFormat{
    UINT, 
    DOUBLE_FLOAT, 
    LONG_DOUBLE, 
    VECTOR 
}; 

struct RegisterInfo{
    RegisterID id; 
    std::string_view name; 
    std::int32_t dwarf_id;
    std::size_t size;
    std::size_t offset; 
    RegisterType type; 
    RegisterFormat format; 
     
}; 

inline constexpr const RegisterInfo g_register_infos[] = {

}; 

} /*namespace XDB*/



}
