

# test/CMakeLists.txt

# Add executable with correct relative path
add_executable(tests tests.cpp)

# Link against libxdb
target_link_libraries(tests PRIVATE libxdb)
