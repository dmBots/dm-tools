cmake_minimum_required(VERSION 4.0)
project(dm_device_sdk_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB src_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(dm_device_sdk_test ${src_files})


target_link_libraries(dm_device_sdk_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdm_device.dll.a)