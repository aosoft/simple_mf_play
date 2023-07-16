cmake_minimum_required(VERSION 3.25)
project(mfplay)

set(CMAKE_CXX_STANDARD 17)

add_library(mfplay mfplay.cpp mfplay_initialize.cpp window_message_queue.cpp)

install (TARGETS mfplay DESTINATION .)