
# tools/CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

add_executable(xdb_exec xdb.cpp)

# Include your project headers
target_include_directories(xdb_exec PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /home/chrinovic/vcpkg/installed/x64-linux/include   # libedit headers
)

# Link against your static library libxdb
target_link_libraries(xdb_exec PRIVATE libxdb)

# Link manually to libedit library
target_link_libraries(xdb_exec PRIVATE
    /home/chrinovic/vcpkg/installed/x64-linux/lib/libedit.a
    # or use libedit.so if you want dynamic linking
)
