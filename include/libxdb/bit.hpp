#pragma once 

#include <cstddef>
#include <cstring>

namespace XDB{

template<class To>
To from_bytes(const std::byte* byte){
    To ret; 
    std::memcpy(&ret, bytes, sizeof(To)); 
    return ret; 
}

template<class From>
std::byte* as_bytes(From &from){
    return reinterpret_cast<std::byte*>(&from); 
}

template<class From>
const std::byte* as_bytes(From& from){
    return reinterpret_cast<const std::byte*>(&from); 
}

}


