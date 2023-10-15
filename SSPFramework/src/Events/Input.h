#pragma once
#include "Events.h"

class Input : private Events
{
    friend class Window;

public:
    // Check if the specified Key is Pressed
    static bool GetKeyDown(KeyCode key);
    // Check if the specified Key is Released
    static bool GetKeyUp(KeyCode key);
    // Check if the specified Key is Hold
    static bool GetKey(KeyCode key);

    // Check if the specified Mouse Button is Pressed
    static bool GetButtonDown(Button button);
    // Check if the specified Mouse Button is Released
    static bool GetButtonUp(Button button);
    // Check if the specified Mouse Button is Hold
    static bool GetButton(Button button);

    // Check if any Key or Mouse Button was Pressed
    static bool AnyKeyDown();
    // Check if any Key or Mouse Button was Released
    static bool AnyKeyUp();
    // Check if any Key or Mouse Button was Hold
    static bool AnyKey();

private:
    inline static bool keyHeld;
    inline static BYTE prevKeyStates[256];
};