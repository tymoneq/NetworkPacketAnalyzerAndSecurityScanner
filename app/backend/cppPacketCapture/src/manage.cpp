#include "../include/manage.h"
#include "../include/logger.h"
#include <memory>
#include "../include/packetCapture.h"
#include <iostream>
#include "../include/networkStructures.h"

using namespace std;

std::unique_ptr<PacketCapturer> capturer;

/* `void initialize()` is a function that initializes the packet capture program. It starts by writing
an informational message to the log and printing a message to the console. It then attempts to find
all available network devices using `pcap_findalldevs()`. If no devices are found or an error
occurs, it logs an error and returns. If devices are found, it selects the first device and creates
a `PacketCapturer` object using the device name. Finally, it frees the device list memory and
catches any exceptions that may occur during initialization, logging an error if needed. */
void initialize()
{
    writeToLog(info, "Starting packet capture program");
    cout << "Starting packet capture\n";

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevices = nullptr;

    if (pcap_findalldevs(&alldevices, errbuf) == -1 || alldevices == nullptr)
    {
        writeToLog(error, "Could not find any devices");
        return;
    }

    cout << "Capturing on device - " << alldevices->name << "\n";

    try
    {
        capturer = std::make_unique<PacketCapturer>(alldevices->name);
        pcap_freealldevs(alldevices); // Free device list after use

    }
    catch (const std::exception &e)
    {
        writeToLog(error, std::string("Error: ") + e.what());
        if (alldevices)
            pcap_freealldevs(alldevices);
    }
}
FeaturePacket getPacket()
{
    return capturer->capturePacket();
}


void stop()
{

    cout << "Stoping\n";

    if (capturer)
        capturer.reset();
    
}