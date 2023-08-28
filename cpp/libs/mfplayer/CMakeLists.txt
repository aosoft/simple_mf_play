cmake_minimum_required(VERSION 3.25)
project(mfplayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

add_library(mfplayer
        initguid.cpp
        async_callback.cpp
        mfplayer_impl.cpp
        mfplayer_initialize.cpp
        window_message_queue.cpp
        mfplayer_create_mfplayer.cpp)

install (TARGETS mfplayer DESTINATION .)