cmake_minimum_required(VERSION 3.10)

project(RubiksCubeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(solver 
    main.cpp
    Models/Rubikscube.cpp
)

target_link_libraries(solver PRIVATE Threads::Threads)