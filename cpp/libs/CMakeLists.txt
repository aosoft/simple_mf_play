cmake_minimum_required(VERSION 3.25)
project(mfplay)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

add_library(mfplay
        initguid.cpp
        mfplay.cpp
        mfplay_initialize.cpp
        window_message_queue.cpp
        mfplay_create_mfplay.cpp)

install (TARGETS mfplay DESTINATION .)