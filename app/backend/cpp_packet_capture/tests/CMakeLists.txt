# Test executable
set(TEST_SOURCES
    testMain.cpp
    testPacketParsing.cpp
    testPacketCapture.cpp
    testPreprocessing.cpp
)

add_executable(packetCaptureTests ${TEST_SOURCES})

target_link_libraries(packetCaptureTests PRIVATE
    packet_capture_lib  # Link with our library
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)
# Make sure the test executable can find the headers
target_include_directories(packetCaptureTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
# Discover tests
include(GoogleTest)
gtest_discover_tests(packetCaptureTests)