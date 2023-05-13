cmake_minimum_required(VERSION 3.10)

project(Calc24 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cqltr SHARED cqltr/cqltr.cpp cqltr/expr.cpp)
add_executable(cqlt console/calc.cpp console/eval.cpp console/main.cpp console/play.cpp)
target_link_libraries(cqlt cqltr)
