#pragma once 

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <sys/user.h>


namespace XDB{

enum class RegisterID{
    #define DEFINE_REGISTER(name,dwarf_id,size,offset,type,format) name 
    #include <libxdb/detail/registers.inc>
    #undef DEFINE_REGISTER
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
    #define DEFINE_REGISTER(name,dwarf_id,size,offset,type,format) \
     {RegisterID::name, #name, dwarf_id, size, offset, type, format}
    #include <libxdb/detail/registers.inc>
    #undef DEFINE_REGISTER
};


template <class F> 
const RegisterInfo& register_info_by(F f){
    auto it = std::find_if(
        std::begin(g_register_infos), 
        std::end(g_register_infos), f); 

    if(it == std::end(g_register_infos))
        XDB::Error::send("Can't find register info"); 

    return *it; 
}

inline const RegisterInfo& register_info_by_id(RegisterID  id){
    return register_info_by([id](auto &i){return i.id == id;}); 
}

inline const RegisterInfo& register_info_by_name(std::string_view name){
    return register_info_by([name](auto& i){return i.name == name;}); 
}
inline const RegisterInfo& register_info_by_dwarf(std::int32_t dwarf_id){
    return register_info_by([dwarf_id](auto& i){return i.dwarf_id == dwarf_id;}); 
}

} /*namespace XDB*/


