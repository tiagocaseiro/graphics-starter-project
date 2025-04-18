#pragma once

#include <cstdio>
#include <utility>

class Logger
{
public:
    template <typename... Args>
    static void log(const int logLevel, Args&&... args)
    {
        if(logLevel <= mLogLevel)
        {
            std::printf(std::forward<Args>(args)...);
            std::fflush(stdout);
        }
    }

    static void setLogLevel(int logLevel);

private:
    static int mLogLevel;
};
