
# test/CMakeLists.txt

add_executable(tests ./tests.cpp)

# Link against libxdb
target_link_libraries(tests PRIVATE libxdb Catch2::Catch2WithMain)

# Register tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(tests)
