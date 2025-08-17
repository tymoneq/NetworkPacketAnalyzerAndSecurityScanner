#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/packetParsing.h"
#include "../include/networkStructures.h"

class PacketParsingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create sample TCP packet
        uint8_t sample_packet[] = {
            // Ethernet header
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55,  // Destination MAC
            0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,  // Source MAC
            0x08, 0x00,                          // EtherType (IPv4)

            // IPv4 header
            0x45, 0x00,                          // Version, IHL
            0x00, 0x28,                          // Total Length
            0x00, 0x00,                          // Identification
            0x40, 0x00,                          // Flags, Fragment Offset
            0x40, 0x06,                          // TTL, Protocol (TCP)
            0x00, 0x00,                          // Checksum
            0xc0, 0xa8, 0x00, 0x01,             // Source IP (192.168.0.1)
            0xc0, 0xa8, 0x00, 0x02,             // Destination IP (192.168.0.2)

            // TCP header
            0x00, 0x50,                          // Source Port (80)
            0x04, 0x00,                          // Destination Port (1024)
            0x00, 0x00, 0x00, 0x00,             // Sequence Number
            0x00, 0x00, 0x00, 0x00,             // Acknowledgment Number
            0x50, 0x02,                          // Data Offset, Flags
            0x20, 0x00,                          // Window Size
            0x00, 0x00,                          // Checksum
            0x00, 0x00                           // Urgent Pointer
        };
        packet_data = std::vector<uint8_t>(sample_packet, sample_packet + sizeof(sample_packet));
    }

    std::vector<uint8_t> packet_data;
};

TEST_F(PacketParsingTest, CheckEthernetHeader) {
    ParsePacket parser(packet_data.data(), packet_data.size());
    EXPECT_TRUE(parser.checkEthHeader(packet_data.size()));
}

TEST_F(PacketParsingTest, CheckIPv4Header) {
    ParsePacket parser(packet_data.data(), packet_data.size());
    EXPECT_TRUE(parser.checkIPv4Header(packet_data.size(), 0x0800));
}

TEST_F(PacketParsingTest, CheckTCPHeader) {
    ParsePacket parser(packet_data.data(), packet_data.size());
    EXPECT_TRUE(parser.checkTCPHeader(packet_data.size(), 20));
}

TEST_F(PacketParsingTest, InvalidPacketSize) {
    std::vector<uint8_t> small_packet(10, 0);
    ParsePacket parser(small_packet.data(), small_packet.size());
    EXPECT_FALSE(parser.checkEthHeader(small_packet.size()));
}