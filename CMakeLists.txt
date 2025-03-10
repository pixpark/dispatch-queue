cmake_minimum_required(VERSION 3.10)
project(DispatchQueueExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(${CMAKE_SOURCE_DIR})

# Add executable
add_executable(DispatchQueueExample
    main.cpp
    dispatch_queue.cc
)