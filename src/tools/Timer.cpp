#include "Timer.h"
void Timer::start()
{
    if(mRunning)
    {
        return;
    }

    mRunning   = true;
    mStartTime = std::chrono::steady_clock::now();
}

float Timer::stop()
{
    if(mRunning == false)
    {
        return 0.0f;
    }

    mRunning = false;

    auto stopTime = std::chrono::steady_clock::now();
    float timerMillisecondsElapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(stopTime - mStartTime).count() / 1000.0f;

    return timerMillisecondsElapsed;
}
