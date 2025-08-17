#ifndef NETWORK_STRUCTURES
#define NETWORK_STRUCTURES

#include <pcap.h>
#include <stdlib.h>
#include <string>
#include <optional>

using namespace std;

#pragma pack(push, 1)

struct EthernetHeader
{
    uint8_t destMac[6];
    uint8_t srcMac[6];
    uint16_t etherType;
};

struct IPv4Header
{
    uint8_t versionIhl;
    uint8_t tos;
    uint16_t totalLength;
    uint16_t id;
    uint16_t flagsFragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t srcIp;
    uint32_t dstIp;
};

struct TCPHeader
{
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint32_t sequenceNumber;
    uint32_t acknowledgmentNumber;
    uint16_t dataOffsetAndFlags;
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
};

struct UDPHeader
{
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t length;
    uint16_t checksum;
};

struct Package
{
    size_t len;
    EthernetHeader ethernet;
    optional<IPv4Header> ip;
    optional<TCPHeader> tcp;
    optional<UDPHeader> udp;
};

struct FeaturePacket
{
    // Layer 2
    uint64_t srcMac; // MAC as integer (only 48 bits used)
    uint64_t dstMac; // MAC as integer (only 48 bits used)
    uint16_t etherType;

    // Layer 3 (IPv4)
    uint32_t srcIp;
    uint32_t dstIp;
    uint8_t protocol;
    uint8_t ttl;
    uint16_t totalLength;
    uint16_t fragmentFlags;

    // Layer 4 (TCP/UDP)
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t lengthOrWindow; // UDP length or TCP window size
    uint8_t tcpFlags;        // Only valid if protocol == TCP

    // Optional derived features (for ML)
    double packetRate = 0;   // packets/sec from this source
    double bytesPerFlow = 0; // average bytes per connection
};

#pragma pack(pop)

#endif