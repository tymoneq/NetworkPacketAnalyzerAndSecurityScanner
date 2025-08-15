#ifndef PREPROCESSING
#define PREPROCESSING

#include <pcap.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "networkStructures.h"

using namespace std;

class Preprocessing
{

private:
    Package package;
    FeaturePacket featurePacket;

public:
    Preprocessing(const Package &package);
    void preprocess();
    uint64_t macToInt(const uint8_t mac[6]);
    ~Preprocessing();
};

#endif