#ifndef PACKET_CAPTURE
#define PACKET_CAPTURE

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class PacketCapturer
{
private:
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    bool isRunning;

public:
    PacketCapturer(const char *deviceName);
    bool openDevice(const char *deviceName);
    bool compileFilter();
    bool applyFilter();
    void capturePackets();
    void packetHandler(const struct pcap_pkthdr &header, const u_char *packet);
    void stop();
    ~PacketCapturer();
};

#endif