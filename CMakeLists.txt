cmake_minimum_required(VERSION 3.9)
project(ntp_client VERSION 1.0.0 
    DESCRIPTION "NTP client example"
    LANGUAGES CXX)
add_compile_options(-Wall -Wextra -W -Wshadow -Wformat -fexceptions)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("src")

file(GLOB SOURCES "src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

