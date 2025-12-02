add_library(libxdb libxdb.cpp) 
add_library(xdb::libxdb ALAIS libxdb)
add_library(libxdb process.cpp)

set_target_properties(
    libxdb
    PROPERTIES OUTPUT_NAME xdb 
)

target_compile_features(libxdb PUBLIC cxx_std_17) 

target_include_features(
    libxdb
    PRIVATE include 
    PUBLIC $(CMAKE_SOURCE_DIR)/include
)


