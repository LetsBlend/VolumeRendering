#pragma once

class Log
{
public:
    template<typename... T>
    static void Trace(T... msg)
    {
        (Log::Print(msg), ...);
        std::cout << std::endl;
    }

    template<typename... T>
    static void Info(T... msg)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        (Log::Print(msg), ...);
        std::cout << std::endl;
    }

    template<typename... T>
    static void Warning(T... msg)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        (Log::Print(msg), ...);
        std::cout << std::endl;
    }

    template<typename... T>
    static void Error(T... msg)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        (Log::Print(msg), ...);
        std::cout << std::endl; 
    }

private:
    template<typename T>
    static void Print(T& msg)
    {
        std::cout << msg;
    }
};

#ifdef ASSERTION_ENABLED
#define DEBUG_ASSERT(x, ...)                                      \
    {                                                             \
        if (!(x))                                                 \
        {                                                         \
            Log::Error(__VA_ARGS__);                              \
            __debugbreak();                                       \
        }                                                         \
    }
#else
#define DEBUG_ASSERT(x, ...)
#endif
