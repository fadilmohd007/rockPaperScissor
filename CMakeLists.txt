# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(StonePaperScissor)

# Add the executable and specify the C file(s) to compile
add_executable(spc main.c)

add_library(randomGen STATIC random_gen.c)
add_library(sqliteConn STATIC sqlite_conn.c)
find_package(SQLite3 REQUIRED)

target_link_libraries(spc randomGen sqliteConn SQLite::SQLite3)


# Optionally, set C standard (e.g., C11)
set(CMAKE_C_STANDARD 11)
