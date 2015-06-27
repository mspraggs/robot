cmake_minimum_required(VERSION 2.8.4)
project(robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

set(SOURCE_FILES
    include/crow_all.h
    include/motor.hpp
    include/robot.hpp
    include/serial.hpp
    include/timer.hpp
    src/main.cpp
    src/motor.cpp
    src/robot.cpp
    src/serial.cpp)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(include ${Boost_INCLUDE_DIRS})

add_executable(robot ${SOURCE_FILES})
target_link_libraries(robot ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
