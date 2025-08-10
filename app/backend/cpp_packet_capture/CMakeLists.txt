cmake_minimum_required(VERSION 3.10)
project(PacketCapture)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)


add_executable(packet_capture main.cpp)

target_link_libraries(packet_capture ${PCAP_LIBRARIES})
target_include_directories(packet_capture PRIVATE ${PCAP_INCLUDE_DIRS})
target_compile_options(packet_capture PRIVATE ${PCAP_CFLAGS_OTHER})




