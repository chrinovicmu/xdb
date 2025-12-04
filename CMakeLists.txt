
cmake_minimum_required(VERSION 3.15)
project(xdb LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include vcpkg toolchain automatically if you haven't passed it on the command line
# set(CMAKE_TOOLCHAIN_FILE "/home/chrinovic/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)
