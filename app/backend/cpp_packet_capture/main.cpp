#include <iostream>
#include <pcap.h>

using namespace std;

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet)
{

    cout << "Captured packet of length " << header->len << " bytes\n";

    for (int i = 0; i < header->len; i++)
        printf("%02x ", packet[i]);

    cout << "\n\n";
}

int main()
{

    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t *handle;

    handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);

    if (handle == NULL)
    {
        cerr << "Couldn't open device\n";
        return -1;
    }

    pcap_loop(handle, -1, packet_handler, NULL);

    pcap_close(handle);

    return 0;
}