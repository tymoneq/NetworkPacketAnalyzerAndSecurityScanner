#include "../include/packetCapture.h"
#include <string>
#include "../include/logger.h"

using namespace std;

PacketCapturer::PacketCapturer(const char *deviceName)
{

    writeToLog(info, "Creating PacketCapture class");

    openDevice(deviceName);
    compileFilter();
    applyFilter();
    capturePackets();
}

PacketCapturer::~PacketCapturer()
{
    pcap_freecode(&this->fp);
    pcap_close(this->handle);
}

bool PacketCapturer::openDevice(const char *deviceName)
{
    this->handle = pcap_open_live(deviceName, BUFSIZ, 1, 1000, this->errbuf);

    if (handle == NULL)
    {
        writeToLog(error, "Could not open any handle");
        return false;
    }

    writeToLog(info, "Successfully opened device");

    return true;
}

bool PacketCapturer::compileFilter()
{

    if (pcap_compile(this->handle, &(this->fp), "tcp", 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        writeToLog(error, "Could not compile filter");
        return false;
    }

    writeToLog(info, "Successfully compiled filters");
    return true;
}

bool PacketCapturer::applyFilter()
{
    if (pcap_setfilter(this->handle, &(this->fp)) == -1)
    {
        writeToLog(error, "Could not apply filter");
        return false;
    }
    writeToLog(info, "Successfully applied filters");

    return true;
}

void PacketCapturer::capturePackets()
{
    writeToLog(info, "Starting capturing packages");
    struct pcap_pkthdr header;
    const u_char *packet;

    while ((packet = pcap_next(this->handle, &header)) != NULL)
    {
        packetHandler(header, packet);
    }
}

void PacketCapturer::packetHandler(const struct pcap_pkthdr &header, const u_char *packet)
{
    printf("Captured a TCP packet with length: %d\n", header.len);
}