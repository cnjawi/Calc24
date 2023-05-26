cmake_minimum_required(VERSION 3.10)

project(Calc24 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("\n[Info] Building on ${CMAKE_HOST_SYSTEM_NAME} platform...")
message("[Info] Currently using ${CMAKE_CXX_COMPILER_ID} compiler...\n")

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES Windows)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
        add_compile_options("-fexec-charset=gbk")
    endif()
endif()

file(GLOB cqltr_src "cqltr/*.cpp")
file(GLOB console_src "console/*.cpp")

add_library(cqltr SHARED ${cqltr_src})
add_executable(cqlt ${console_src})
target_link_libraries(cqlt cqltr)
