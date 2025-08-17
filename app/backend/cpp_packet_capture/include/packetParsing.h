#ifndef PACKET_PARSING
#define PACKET_PARSING

#include <pcap.h>
#include <stdlib.h>
#include <string>
#include "networkStructures.h"

class ParsePacket
{
private:
    FeaturePacket preprocessedPacket;

public:
    ParsePacket(const uint8_t *data, size_t len);
    ~ParsePacket();
    void parsePacket(const uint8_t *data, size_t len);
    bool checkEthHeader(const size_t &len);
    bool checkIPv4Header(const size_t &len, const uint16_t &etherType);
    bool checkTCPHeader(const size_t &len, const uint8_t &ipHeaderLength);
    bool checkUDPHeader(const size_t &len, const uint8_t &ipHeaderLength);
    FeaturePacket getPreprocessedPacket() const;
};

#endif