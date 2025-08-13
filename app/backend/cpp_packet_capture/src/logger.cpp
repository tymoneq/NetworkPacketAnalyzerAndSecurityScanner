#include "../include/logger.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <string>

using namespace std;

std::shared_ptr<spdlog::logger> fileLogger = spdlog::rotating_logger_mt("rotating_logger", "logs.log", 1048576 * 5, 3);

void writeToLog(const MessagesTypes &type, const string &message)
{

    switch (type)
    {
    case info:
        fileLogger->info(message);
        break;

    case critical:
        fileLogger->critical(message);
        break;

    case warn:
        fileLogger->warn(message);
        break;

    case error:
        fileLogger->error(message);
        break;

    case trace:
        fileLogger->trace(message);
        break;

    case debug:
        fileLogger->debug(message);
        break;

    default:
        break;
    }

    fileLogger->flush();
}