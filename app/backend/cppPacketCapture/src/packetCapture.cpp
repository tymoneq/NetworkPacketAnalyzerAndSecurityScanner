#include "../include/packetCapture.h"
#include <string>
#include "../include/logger.h"
#include "../include/packetParsing.h"
#include "../include/manage.h"
#include <stdexcept>

using namespace std;

/* The `PacketCapturer::PacketCapturer(const char *deviceName)` constructor is initializing a
`PacketCapturer` object by performing the following steps:
1. Writing an informational log message indicating the creation of the `PacketCapture` class.
2. Attempting to open a live capture device using the provided `deviceName`.
3. If the device opening fails, it throws a `std::runtime_error` with a message indicating the
failure.
4. Compiling a filter to capture only TCP packets.
5. If the filter compilation fails, it closes the handle and throws a `std::runtime_error` with a
corresponding message.
6. Applying the compiled filter to the capture device.
7. If applying the filter fails, it frees the filter code, closes the handle, and throws a
`std::runtime_error` with a message. */
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

/* The `bool PacketCapturer::openDevice(const char *deviceName)` function is responsible for opening a
live capture device using the provided `deviceName`. Here is a breakdown of what it does: */
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

/* `bool PacketCapturer::compileFilter()` is a member function of the `PacketCapturer` class in C++.
This function is responsible for compiling a filter to capture only TCP packets. Here is a breakdown
of what it does: */
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

/* `bool PacketCapturer::applyFilter()` is a member function of the `PacketCapturer` class in C++. This
function is responsible for applying the compiled filter to the capture device. Here is a breakdown
of what it does: */
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
/* The `FeaturePacket PacketCapturer::capturePacket()` function is a member function of the
`PacketCapturer` class in C++. This function is responsible for capturing a single packet from the
live capture device that has been opened and applying a packet handler to process the captured
packet. */

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
/* `void PacketCapturer::stop()` is a member function of the `PacketCapturer` class in C++. This
function is responsible for stopping the packet capture process. Here is a breakdown of what it
does: */
void PacketCapturer::stop()
{
    writeToLog(info, "Stopping packet capture");
    if (this->handle)
        pcap_breakloop(this->handle);
}

/* The `FeaturePacket PacketCapturer::packetHandler(const struct pcap_pkthdr &header, const u_char
*packet)` function is a member function of the `PacketCapturer` class in C++. This function is
responsible for processing a captured packet by creating a `ParsePacket` object with the packet data
and length obtained from the `pcap_pkthdr` structure. */
FeaturePacket PacketCapturer::packetHandler(const struct pcap_pkthdr &header, const u_char *packet)
{

    writeToLog(info, "Captured a TCP packet with length: " + to_string(header.len));
    ParsePacket parsePacket(packet, header.len);

    return parsePacket.getPreprocessedPacket();
}