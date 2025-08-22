#include "../include/packetParsing.h"
#include "../include/logger.h"
#include "../include/networkStructures.h"
#include "../include/preprocessing.h"
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>
#include <pcap.h>
#include <stdlib.h>

using namespace std;

/**
 * This function initializes a ParsePacket object with the provided data and length.
 * 
 * Parameters
 * ----------
 * data : uint8_t
 * 	The `data` parameter in the `ParsePacket` constructor is a pointer to an array of `uint8_t` data,
 * which represents the packet data that needs to be parsed.
 * len : size_t
 * 	The `len` parameter in the `ParsePacket` constructor represents the length of the data array that
 * is being passed to the constructor. It indicates the number of elements in the `data` array that
 * need to be processed by the `ParsePacket` constructor.
 */
ParsePacket::ParsePacket(const uint8_t *data, size_t len)
{
    if (data == nullptr)
    {
        writeToLog(error, "Received null packet data");
        return;
    }

    this->preprocessedPacket = {};

    parsePacket(data, len);
}

/**
 * This function is used to parse a packet of data represented by a uint8_t array.
 * 
 * Parameters
 * ----------
 * data : uint8_t
 * 	Data is a pointer to an array of unsigned 8-bit integers (uint8_t) which represents the packet data
 * to be parsed.
 * len : size_t
 * 	The `len` parameter represents the length of the data array that is being passed to the
 * `parsePacket` function. It indicates the number of elements in the array that need to be processed
 * by the function.
 */
void ParsePacket::parsePacket(const uint8_t *data, size_t len)
{

    writeToLog(info, "Starting parsing packet");

    if (!checkEthHeader(len))
        return;

    const EthernetHeader *eth = reinterpret_cast<const EthernetHeader *>(data);

    // converting ethertype from network byte order
    uint16_t etherType = ntohs(eth->etherType);

    if (!checkIPv4Header(len, etherType))
        return;

    const IPv4Header *ip = reinterpret_cast<const IPv4Header *>(data + sizeof(EthernetHeader));

    uint8_t ipHeaderProtocol = ip->protocol;
    uint8_t ipHeaderLength = (ip->versionIhl & 0x0F) * 4;

    Package package;
    package.len = len;
    package.ethernet = (*eth);
    package.ip = (*ip);

    if (ipHeaderProtocol == 6)
    {
        if (!checkTCPHeader(len, ipHeaderLength))
            return;

        const TCPHeader *tcp = reinterpret_cast<const TCPHeader *>(data + sizeof(EthernetHeader) + ipHeaderLength);

        writeToLog(info, "TCP packet parsed");

        package.tcp = (*tcp);
    }

    else if (ipHeaderProtocol == 17)
    {
        if (!checkUDPHeader(len, ipHeaderLength))
            return;

        const UDPHeader *udp = reinterpret_cast<const UDPHeader *>(data + sizeof(EthernetHeader) + ipHeaderLength);

        writeToLog(info, "UDP packet parsed");

        package.udp = (*udp);
    }

    Preprocessing processor(package);

    this->preprocessedPacket = processor.getPacket();
}

/* The `bool ParsePacket::checkEthHeader(const size_t &len)` function is checking the presence of an
Ethernet header in the packet data. Here's a breakdown of what it does: */
bool ParsePacket::checkEthHeader(const size_t &len)
{
    writeToLog(info, "Checking ethernet header...");

    if (len < sizeof(EthernetHeader))
    {
        writeToLog(error, "No ethernet header detected");
        return false;
    }

    writeToLog(info, "Ethernet header detected");
    return true;
}

/* The `bool ParsePacket::checkIPv4Header(const size_t &len, const uint16_t &etherType)` function is
checking for the presence of an IPv4 header in the packet data. Here's a breakdown of what it does: */
bool ParsePacket::checkIPv4Header(const size_t &len, const uint16_t &etherType)
{
    writeToLog(info, "Checking ipv4 header...");

    if ((len < sizeof(EthernetHeader) + sizeof(IPv4Header)) || etherType != 0x0800)
    {
        writeToLog(error, "No ipv4 header detected");
        return false;
    }

    writeToLog(info, "Ipv4 header detected");
    return true;
}

/* The `bool ParsePacket::checkTCPHeader(const size_t &len, const uint8_t &ipHeaderLength)` function is
responsible for checking the presence of a TCP header in the packet data. Here's a breakdown of what
it does: */
bool ParsePacket::checkTCPHeader(const size_t &len, const uint8_t &ipHeaderLength)
{
    writeToLog(info, "Checking tcp header...");

    if (len < sizeof(EthernetHeader) + sizeof(TCPHeader) + ipHeaderLength)
    {
        writeToLog(error, "No tcp header detected");
        return false;
    }

    writeToLog(info, "Tcp header detected");
    return true;
}

/* The `bool ParsePacket::checkUDPHeader(const size_t &len, const uint8_t &ipHeaderLength)` function is
responsible for checking the presence of a UDP header in the packet data. Here's a breakdown of what
it does: */
bool ParsePacket::checkUDPHeader(const size_t &len, const uint8_t &ipHeaderLength)
{
    writeToLog(info, "Checking udp header...");

    if (len < sizeof(EthernetHeader) + sizeof(UDPHeader) + ipHeaderLength)
    {
        writeToLog(error, "No udp header detected");
        return false;
    }

    writeToLog(info, "Udp header detected");
    return true;
}
/**
 * This function returns a preprocessed feature packet.
 */

FeaturePacket ParsePacket::getPreprocessedPacket() const
{
    return this->preprocessedPacket;
}

ParsePacket::~ParsePacket() {}