#pragma once
#include <chrono>

using namespace std;
using namespace chrono;
#define TOLERANCE 1'020'000
#define PERIOD 1

class Time
{
public:
    inline static float timeScale = 1;
    // Average fps of the last 20 frames
    inline static int fps = 0;
    // Average milliseconds of the last 20 frames
    inline static float milliSeconds = 0;
    inline static double deltaTime = 1;
    inline static double realTimeSinceStartUp = 0;
    inline static double timeSinceStartUp = 0;

    static void Sleep(double seconds);

    static void UpdateStartTime();
    static void UpdateEndTime();

    void StartUp() {}
    void ShutDown() {}

private:
    static std::chrono::system_clock::time_point startTime;
    static std::chrono::system_clock::time_point endTime;
};

