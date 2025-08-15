#include <iostream>
#include <string>
#include "../include/packetCapture.h"
#include "../include/logger.h"
#include "../include/start.h"
#include "../include/stopAtomic.h"

using namespace std;

Start::Start()
{
    start();
}

// void Start::start()
void start()
{

    cout << "Starting ...\n";
    writeToLog(info, "Starting packet capture program");

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevices = nullptr;

    if (pcap_findalldevs(&alldevices, errbuf) == -1)
    {
        writeToLog(error, "Could not find any device");
        return;
    }

    // Check if we have at least one device
    if (alldevices == nullptr)
    {
        writeToLog(error, "No devices found");
        return;
    }

    writeToLog(info, string("Successfully found devices") + (alldevices->name));

    cout << "Successfully found devices ...\n";
    cout << "Starting capturing on " << alldevices->name << "\n";
    PacketCapturer packetCapture(alldevices->name);

    writeToLog(info, "Closing packet capture program");
    pcap_freealldevs(alldevices);
}

void stop()
{
    stopCapture.store(true);
}


Start::~Start()
{
}