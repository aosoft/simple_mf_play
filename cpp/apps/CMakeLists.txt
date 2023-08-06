cmake_minimum_required(VERSION 3.25)
project(mfplaytest)

set(CMAKE_CXX_STANDARD 17)

include_directories(../libs)

add_executable(mfplaytest WIN32 main.cpp main_window.cpp)

target_link_libraries(mfplaytest mfplay)
