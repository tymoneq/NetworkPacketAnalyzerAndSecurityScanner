#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include "spdlog/spdlog.h"
#include <string>
using namespace std;

// Declare the logger as extern so other files know about it
extern std::shared_ptr<spdlog::logger> fileLogger;

enum MessagesTypes
{
    info,
    critical,
    warn,
    error,
    trace,
    debug

};

void writeToLog(const MessagesTypes &type, const string &message);

#endif // LOGGER_H
