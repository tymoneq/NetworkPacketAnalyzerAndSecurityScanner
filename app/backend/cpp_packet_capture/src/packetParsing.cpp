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

ParsePacket::ParsePacket(const uint8_t *data, size_t len)
{
    parsePacket(data, len);
}

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
}

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

ParsePacket::~ParsePacket() {}