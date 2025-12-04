
# tools/CMakeLists.txt

add_executable(xdb_exec xdb.cpp)

# Link against libxdb
target_link_libraries(xdb_exec PRIVATE libxdb)
