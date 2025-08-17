#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/packetCapture.h"

class PacketCaptureTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

TEST_F(PacketCaptureTest, OpenDevice) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs;
    
    // Should be able to find devices
    ASSERT_EQ(pcap_findalldevs(&alldevs, errbuf), 0) 
        << "Failed to find network devices: " << errbuf;
    
    ASSERT_NE(alldevs, nullptr) 
        << "No network interfaces found";
    
    // Should be able to create PacketCapturer with valid device
    EXPECT_NO_THROW({
        PacketCapturer capturer(alldevs->name);
    });
    
    pcap_freealldevs(alldevs);
}

TEST_F(PacketCaptureTest, InvalidDevice) {
    EXPECT_THROW({
        PacketCapturer capturer("invalid_device_name");
    }, std::runtime_error);
}

TEST_F(PacketCaptureTest, FilterCompilation) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs;
    ASSERT_EQ(pcap_findalldevs(&alldevs, errbuf), 0);
    
    PacketCapturer capturer(alldevs->name);
    EXPECT_TRUE(capturer.compileFilter());
    
    pcap_freealldevs(alldevs);
}