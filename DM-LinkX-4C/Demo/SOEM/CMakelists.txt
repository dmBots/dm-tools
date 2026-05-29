cmake_minimum_required(VERSION 3.20)
project(linkx_soem_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB src_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linkx/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/linkx)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/soem)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/npcap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/npcap/pcap)



add_executable(linkx_soem_demo ${src_files})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/npcap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/npcap/pcap)

target_link_libraries(linkx_soem_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/soem/soem.lib)
target_link_libraries(linkx_soem_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/npcap/Packet.lib)
target_link_libraries(linkx_soem_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/npcap/wpcap.lib)

target_link_libraries(linkx_soem_demo PRIVATE winmm)
target_link_libraries(linkx_soem_demo PRIVATE WS2_32)
