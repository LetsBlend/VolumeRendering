#include "pch.h"
#include "SSPTime.h"
#include "Engine.h"

// This code is from Blat Blatnik, thank you Blat VERY COOL
// Check him out over here: https://blog.bearcats.nl/perfect-sleep-function/
void Time::Sleep(double seconds)
{
    auto t = high_resolution_clock::now();
    auto target = t + nanoseconds(int64_t(seconds * 1e9));

    static HANDLE timer;
    if (!timer)
        timer = CreateWaitableTimerExW(NULL, NULL,
            CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
            TIMER_ALL_ACCESS);

    int64_t maxTicks = PERIOD * 9'500;
    for (;;) {
        int64_t remaining = (target - t).count();
        int64_t ticks = (remaining - TOLERANCE) / 100;
        if (ticks <= 0)
            break;
        if (ticks > maxTicks)
            ticks = maxTicks;

        LARGE_INTEGER due;
        due.QuadPart = -ticks;
        SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
        WaitForSingleObject(timer, INFINITE);
        t = high_resolution_clock::now();
    }

    // spin
    while (high_resolution_clock::now() < target)
        YieldProcessor();
}

void Time::UpdateStartTime()
{
    startTime = std::chrono::system_clock::now();
}

void Time::UpdateEndTime()
{
    // calculate time it took for engine to excecute
    endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    // Thottle the gameEngine if the engine has more fps than the target FrameRate
    double targetFrameRate = 1.0 / (double)Engine::targetFrameRate;
    if (elapsedTime.count() < targetFrameRate)
        Sleep(targetFrameRate - elapsedTime.count());

    // Calculate deltaTime and other
    endTime = std::chrono::system_clock::now();
    elapsedTime = endTime - startTime;
    deltaTime = elapsedTime.count();
    milliSeconds = (float)elapsedTime.count() * 1000;
}
std::chrono::system_clock::time_point Time::startTime;
std::chrono::system_clock::time_point Time::endTime;