#include <iostream>
#include <fstream>
#include <string>
#include "include/packetCapture.h"
#include "include/logger.h"

using namespace std;

void start()
{

    cout << "Starting ...\n";
    writeToLog(info, "Starting packet capture program");

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevices;

    if (pcap_findalldevs(&alldevices, errbuf) == -1)
    {
        writeToLog(error, "Could not find any device");
        return;
    }

    writeToLog(info, "Successfully found devices");

    cout << "Successfully found devices ...\n";
    cout << "Starting capturing\n";
    PacketCapturer packetCapture(alldevices->name);

    writeToLog(info, "Closing packet capture program");
    pcap_freealldevs(alldevices);
}

int main()
{
    start();

    return 0;
}
