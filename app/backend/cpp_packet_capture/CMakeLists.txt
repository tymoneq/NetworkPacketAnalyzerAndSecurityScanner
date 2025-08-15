cmake_minimum_required(VERSION 3.10)
project(PacketCapture)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PCAP REQUIRED libpcap)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Different configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

find_package(spdlog QUIET)

if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found, downloading...")
    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
    )
    FetchContent_MakeAvailable(spdlog)
endif()


add_executable(packet_capture
                main.cpp
                src/preprocessing.cpp 
                src/packetCapture.cpp 
                src/packetParsing.cpp
                src/logger.cpp
                )


target_link_libraries(packet_capture ${PCAP_LIBRARIES} spdlog::spdlog)
target_include_directories(packet_capture PRIVATE ${PCAP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include())
target_compile_options(packet_capture PRIVATE ${PCAP_CFLAGS_OTHER})



