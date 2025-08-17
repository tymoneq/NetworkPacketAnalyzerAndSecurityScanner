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

FeaturePacket PacketCapturer::capturePacket()
{
    writeToLog(info, "Starting capturing package");
    struct pcap_pkthdr header;
    const u_char *packet;

    packet = pcap_next(this->handle, &header);
    if (packet)
        return packetHandler(header, packet);
    return FeaturePacket{};
}
void PacketCapturer::stop()
{
    writeToLog(info, "Stopping packet capture");
    if (this->handle)
        pcap_breakloop(this->handle);
}

FeaturePacket PacketCapturer::packetHandler(const struct pcap_pkthdr &header, const u_char *packet)
{

    writeToLog(info, "Captured a TCP packet with length: " + to_string(header.len));
    ParsePacket parsePacket(packet, header.len);

    return parsePacket.getPreprocessedPacket();
}