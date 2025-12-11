#include <cstddef>
#include <cstdint>
#include <libxdb/registers.hpp>
#include <libxdb/bit.hpp> 

XDB::Registers::value XDB::Registers::read(const RegisterInfo& info) const {
    auto bytes = _as_bytes(_data); 

    if(info.size == XDB::RegisterFormat::UINT){

        switch(info.size){
            case 1: 
                return from_bytes<std::uint8_t>(bytes + info.offset); 
            case 2: 
                return from_bytes<std::uint16_t>(bytes + info.offset); 
            case 4:
                return from_bytes<std::uint32_t>(bytes + info.offset); 
            case 8:
                return from_bytes<std::uint64_t>(bytes + info.offset); 
            default: 
                XDB::Error::send("unexpected register size"); 
        }
    }
    else if(info.format == XDB::RegisterFormat::DOUBLE_FLOAT){
        return from_bytes<double>(bytes + info.offset); 
    }
    else if(info.format == XDB::RegisterFormat::LONG_DOUBLE){
        return from_bytes<long double>(bytes + info.offset);
    }
    else if(info.format == XDB::RegisterFormat::VECTOR and info.size == 8){
        return from_bytes<byte64>(bytes + info.offset);
    }
    else {
        return from_bytes<byte128>(bytes + info.offset); 
    }
}

void XDB::Registers::write(const RegisterInfo& info, value val){
    auto bytes = as_bytes(_data); 

    std::visit([&](auto & v){
        if(sizeof(v) == info.size){
            auto val_bytes = as_bytes(v); 
            std::copy(val_bytes, val_bytes + sizeof(v), bytes + info.offset); 
        } 
        else{
            std::cerr << "XDB::Registers::write called with mismatched register and value size"; 
            std::terminate(); 
        }
    },val);

    _proc->write_user_area(info.offset, from_bytes(std::uint64_t(bytes + info.offset)); 
}
