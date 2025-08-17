#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/preprocessing.h"
#include "../include/networkStructures.h"

class PreprocessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        package.len = 64;
        
        // Set up Ethernet header
        memset(package.ethernet.destMac, 0x11, 6);
        memset(package.ethernet.srcMac, 0x22, 6);
        package.ethernet.etherType = htons(0x0800);
        
        // Set up IPv4 header
        IPv4Header ipv4;
        ipv4.protocol = 6;  // TCP
        ipv4.srcIp = htonl(0xC0A80001);  // 192.168.0.1
        ipv4.dstIp = htonl(0xC0A80002);  // 192.168.0.2
        package.ip = ipv4;
        
        // Set up TCP header
        TCPHeader tcp;
        tcp.sourcePort = htons(80);
        tcp.destinationPort = htons(443);
        package.tcp = tcp;
    }

    Package package;
};

TEST_F(PreprocessingTest, PackageInitialization) {
    EXPECT_NO_THROW({
        Preprocessing prep(package);
    });
}

TEST_F(PreprocessingTest, BasicPreprocessing) {
    Preprocessing prep(package);
    EXPECT_NO_THROW(prep.preprocess());
}

TEST_F(PreprocessingTest, TCPPacketFeatures) {
    Preprocessing prep(package);
    prep.preprocess();
    
    // Add assertions for your preprocessing results
    // You'll need to add getters to your Preprocessing class
    // EXPECT_EQ(prep.getSourcePort(), 80);
    // EXPECT_EQ(prep.getDestinationPort(), 443);
}