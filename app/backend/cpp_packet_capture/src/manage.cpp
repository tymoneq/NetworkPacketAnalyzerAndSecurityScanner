#include "../include/manage.h"
#include "../include/logger.h"
#include <memory>
#include "../include/packetCapture.h"
#include <iostream>
#include "../include/networkStructures.h"

using namespace std;

std::unique_ptr<PacketCapturer> capturer;

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
    {
        capturer->stop();
        capturer.reset();
    }
}