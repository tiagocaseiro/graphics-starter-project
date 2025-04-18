#include "Logger.h"

#include <algorithm>

int Logger::mLogLevel = 1;

void Logger::setLogLevel(int logLevel)
{
    static constexpr auto MAX_LEVEL = 9;

    mLogLevel = std::min(MAX_LEVEL, logLevel);
}