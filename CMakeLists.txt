cmake_minimum_required(VERSION 3.16)
project(ParallelQuickSort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} 
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE Threads::Threads)

add_subdirectory(examples)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Optimization flags: ${OPTIMIZATION_FLAGS}")
message(STATUS "Hardware threads: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
