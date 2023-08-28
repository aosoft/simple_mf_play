cmake_minimum_required(VERSION 3.25)
project(mfplayertest)

set(CMAKE_CXX_STANDARD 17)

include_directories(../libs/mf)

add_executable(mfplayertest WIN32 main.cpp main_window.cpp)

target_link_libraries(mfplayertest mfplayer)
