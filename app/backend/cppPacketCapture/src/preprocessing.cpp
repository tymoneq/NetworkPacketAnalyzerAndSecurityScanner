#include "../include/preprocessing.h"
#include "../include/logger.h"
#include "../include/networkStructures.h"
#include <pcap.h>
#include <stdlib.h>
#include <string>

using namespace std;

/**
 * This function initializes the Preprocessing object with the provided Package object.
 * 
 * Parameters
 * ----------
 * package : Package
 * 	It seems like you were about to provide some information about the `package` parameter. Could you
 * please provide more details or let me know how I can assist you further with the `Preprocessing`
 * constructor?
 */
Preprocessing::Preprocessing(const Package &package)
{
    this->package = package;
    preprocess();
}

/* The `inline uint64_t Preprocessing::macToInt(const uint8_t mac[6])` function is converting a MAC
address represented as an array of 6 bytes (uint8_t) into a single 64-bit integer (uint64_t). */
inline uint64_t Preprocessing::macToInt(const uint8_t mac[6])
{
    return ((uint64_t)mac[0] << 40) |
           ((uint64_t)mac[1] << 32) |
           ((uint64_t)mac[2] << 24) |
           ((uint64_t)mac[3] << 16) |
           ((uint64_t)mac[4] << 8) |
           (uint64_t)mac[5];
}
/* The `void Preprocessing::preprocess()` function is responsible for processing the data from the
`Package` object and populating the `FeaturePacket` object with the extracted information. Here is a
breakdown of what the function is doing: */
void Preprocessing::preprocess()
{
    writeToLog(info, "Starting preprocessing ...");

    // Ethernet
    this->featurePacket.srcMac = macToInt(this->package.ethernet.srcMac);
    this->featurePacket.dstMac = macToInt(this->package.ethernet.destMac);
    this->featurePacket.etherType = ntohs(package.ethernet.etherType);

    // Ipv4
    this->featurePacket.srcIp = ntohs(this->package.ip->srcIp);
    this->featurePacket.dstIp = ntohs(this->package.ip->dstIp);
    this->featurePacket.protocol = this->package.ip->protocol;
    this->featurePacket.ttl = this->package.ip->ttl;
    this->featurePacket.totalLength = ntohs(this->package.ip->totalLength);
    this->featurePacket.fragmentFlags = ntohs(this->package.ip->flagsFragmentOffset);

    // TCP
    if (this->featurePacket.protocol == 6)
    {
        this->featurePacket.sourcePort = ntohs(this->package.tcp->sourcePort);
        this->featurePacket.destinationPort = ntohs(this->package.tcp->destinationPort);
        this->featurePacket.lengthOrWindow = ntohs(this->package.tcp->windowSize);
        this->featurePacket.tcpFlags = ntohs(this->package.tcp->dataOffsetAndFlags) & 0x3F;
    }
    // UDP
    else if (this->featurePacket.protocol == 17)
    {
        this->featurePacket.sourcePort = ntohs(this->package.udp->sourcePort);
        this->featurePacket.destinationPort = ntohs(this->package.udp->destinationPort);
        this->featurePacket.lengthOrWindow = ntohs(this->package.udp->length);
        this->featurePacket.tcpFlags = 0;
    }

    writeToLog(info, "Finished preprocessing");
}

/* The `FeaturePacket Preprocessing::getPacket() const` function is a member function of the
`Preprocessing` class that returns a constant reference to the `FeaturePacket` object stored within
the `Preprocessing` instance. By using the `const` keyword at the end of the function declaration,
it indicates that this member function does not modify the state of the `Preprocessing` object on
which it is called. */
FeaturePacket Preprocessing::getPacket() const
{
    return this->featurePacket;
}

Preprocessing::~Preprocessing()
{
}
