
# src/CMakeLists.txt

# Define library with all source files in src/
add_library(libxdb
    process.cpp
)

# Optional alias for imported-style target
add_library(xdb::libxdb ALIAS libxdb)

# Output name
set_target_properties(libxdb PROPERTIES OUTPUT_NAME xdb)

# Include directories (so #include <libxdb/...> works)
target_include_directories(
    libxdb
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# C++ standard
target_compile_features(libxdb PUBLIC cxx_std_17)
