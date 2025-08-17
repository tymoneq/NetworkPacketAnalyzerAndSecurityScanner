#include "../include/packetCapture.h"
#include <string>
#include "../include/logger.h"
#include "../include/packetParsing.h"
#include "../include/manage.h"
#include <stdexcept>

using namespace std;

PacketCapturer::PacketCapturer(const char *deviceName)
{

    writeToLog(info, "Creating PacketCapture class");

    if (!openDevice(deviceName))
        throw ::std::runtime_error(string("Failed to open device"));

    if (!compileFilter())
    {
        pcap_close(handle);
        throw ::std::runtime_error(string("Failed to compile filter"));
    }
    if (!applyFilter())
    {
        pcap_freecode(&fp);
        pcap_close(handle);
        throw ::std::runtime_error(string("Failed to apply filter"));
    }
}

PacketCapturer::~PacketCapturer()
{
    stop();
    if (handle)
    {
        pcap_freecode(&this->fp);
        pcap_close(this->handle);
        this->handle = nullptr;
    }
}

bool PacketCapturer::openDevice(const char *deviceName)
{
    if (!deviceName)
    {
        writeToLog(error, "Device name is null");
        return false;
    }

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
    this->isRunning = true;
    writeToLog(info, "Starting capturing packages");
    struct pcap_pkthdr header;
    const u_char *packet;

    while (((packet = pcap_next(this->handle, &header)) != NULL))
    {
        if (packet && isRunning)
        {
            packetHandler(header, packet);
        }
    }
}
void PacketCapturer::stop()
{
    this->isRunning = false;
    writeToLog(info, "Stopping packet capture");
    if (this->handle)
        pcap_breakloop(this->handle);
}

void PacketCapturer::packetHandler(const struct pcap_pkthdr &header, const u_char *packet)
{

    writeToLog(info, "Captured a TCP packet with length: " + to_string(header.len));
    ParsePacket(packet, header.len);
}