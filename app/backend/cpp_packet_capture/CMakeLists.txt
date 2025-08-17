find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
cmake_minimum_required(VERSION 3.10)
project(PacketCapture)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PCAP REQUIRED libpcap)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
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

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()


# Python module
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.13.1
)
FetchContent_MakeAvailable(pybind11)

# Shared code files
set(SRC_FILES
    src/preprocessing.cpp
    src/packetCapture.cpp
    src/packetParsing.cpp
    src/logger.cpp
    src/manage.cpp
)

# Create a static library from source files
add_library(packet_capture_lib STATIC ${SRC_FILES})
target_include_directories(packet_capture_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PCAP_INCLUDE_DIRS}
)
target_link_libraries(packet_capture_lib PUBLIC 
    ${PCAP_LIBRARIES} 
    spdlog::spdlog
)

# Standalone executable (optional)
add_executable(packetCapture main.cpp ${SRC_FILES})
target_link_libraries(packetCapture PRIVATE packet_capture_lib)
target_include_directories(packetCapture PRIVATE ${PCAP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(packetCapture PRIVATE ${PCAP_CFLAGS_OTHER})

# Output the compiled .so directly into python_backend
set(PYTHON_MODULE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../python_backend)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULE_OUTPUT_DIR})

# Python module
pybind11_add_module(packetCapturePy _pybind/pybind.cpp ${SRC_FILES})
target_link_libraries(packetCapturePy PRIVATE packet_capture_lib)
target_include_directories(packetCapturePy PRIVATE ${PCAP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(packetCapturePy PRIVATE ${PCAP_CFLAGS_OTHER})


# Add tests subdirectory
add_subdirectory(tests)