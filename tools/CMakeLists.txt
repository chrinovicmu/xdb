
cmake_minimum_required(VERSION 3.15)
project(xdb_exec_project LANGUAGES C CXX)

# ==== Executable ====
add_executable(xdb_exec xdb.cpp)

# ==== Includes ====
target_include_directories(xdb_exec PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /home/chrinovic/vcpkg/installed/x64-linux/include  # libedit headers
)

# ==== Link project core library ====
target_link_libraries(xdb_exec PRIVATE
    libxdb
)

# ==== Link libedit + terminfo + ncurses (fixes undefined references) ====
target_link_libraries(xdb_exec PRIVATE
    /home/chrinovic/vcpkg/installed/x64-linux/lib/libedit.a
    tinfo        # <-- Required! Provides tputs, tgetstr, tgoto, etc.
    ncurses      # Sometimes required alongside tinfo — safe to include
)
