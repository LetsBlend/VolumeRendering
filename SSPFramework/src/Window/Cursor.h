#pragma once
enum class CursorMode
{
    None = 1,
    Locked = 2,
    Hidden = 0,
    LockedAndHidden = 0
};

class Cursor
{
public:
    inline static CursorMode cursorMode = CursorMode::None;
};

