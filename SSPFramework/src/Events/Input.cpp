#include "pch.h"
#include "Input.h"

bool Input::GetKeyDown(KeyCode key)
{
    BYTE keyState = BYTE(GetKeyState((int)key));
    BYTE prevKeyState = Input::prevKeyStates[(int)key];
    if (keyState > prevKeyState)
        return true;
    return false;
}
bool Input::GetKeyUp(KeyCode key)
{
    BYTE keyState = BYTE(GetKeyState((int)key));
    BYTE prevKeyState = Input::prevKeyStates[(int)key];
    if (keyState < prevKeyState)
        return true;
    return false;
}
bool Input::GetKey(KeyCode key)
{
    BYTE keyState = BYTE(GetKeyState((int)key));
    return keyState > 1 ? true : false;
}

bool Input::GetButtonDown(Button mouseButton)
{
    BYTE keyState = BYTE(GetKeyState((int)mouseButton));
    BYTE prevKeyState = Input::prevKeyStates[(int)mouseButton];
    if (keyState > prevKeyState)
        return true;
    return false;
}
bool Input::GetButtonUp(Button mouseButton)
{
    BYTE keyState = BYTE(GetKeyState((int)mouseButton));
    BYTE prevKeyState = Input::prevKeyStates[(int)mouseButton];
    if (keyState < prevKeyState)
        return true;
    return false;
}
bool Input::GetButton(Button mouseButton)
{
    BYTE buttonState = BYTE(GetKeyState((int)mouseButton));

    return buttonState > 1 ? true : false;
}

bool Input::AnyKeyDown()
{
    for (int i = 0; i < 256; i++)
    {
        BYTE keyState = BYTE(GetKeyState(i));
        BYTE prevKeyState = Input::prevKeyStates[i];
        if (keyState > prevKeyState)
            return true;
    }

    return false;
}
bool Input::AnyKeyUp()
{
    for (int i = 0; i < 256; i++)
    {
        BYTE keyState = BYTE(GetKeyState(i));
        BYTE prevKeyState = Input::prevKeyStates[i];
        if (keyState < prevKeyState)
            return true;
    }

    return false;
}
bool Input::AnyKey()
{
    for (int i = 0; i < 256; i++)
    {
        BYTE keyState = BYTE(GetKeyState(i));
        if (keyState > 1)
            return true;
    }

    return false;
}