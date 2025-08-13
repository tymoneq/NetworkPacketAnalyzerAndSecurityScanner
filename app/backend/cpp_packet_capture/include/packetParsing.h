#ifndef PACKET_PARSING
#define PACKET_PARSING

#include <pcap.h>
#include <stdlib.h>
#include <string>

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

#pragma pack(pop)

class ParsePacket
{
private:
public:
    ParsePacket(const uint8_t *data, size_t len);
    ~ParsePacket();
    void parsePacket(const uint8_t *data, size_t len);
    bool checkEthHeader(const size_t &len);
    bool checkIPv4Header(const size_t &len, const uint16_t &etherType);
    bool checkTCPHeader(const size_t &len, const uint8_t &ipHeaderLength);
    bool checkUDPHeader(const size_t &len, const uint8_t &ipHeaderLength);
};

#endif