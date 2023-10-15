#include "pch.h"
#include "Window.h"
#include "Cursor.h"
#include "Events/Input.h"
#include "Renderer/D3D11.h"
#include "Renderer/Camera.h"

// GUI
#include "imgui.h"
#include "ImGui/ImGuiDXRenderer.h"
#include "ImGui/ImGuiWinRenderer.h"

void Window::Initialize()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    // LPSTR lpCmdLine = GetCommandLineA();

    const auto pClassName = L"EngineWindow";
    // register Window class
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = Window::WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;
    RegisterClassExW(&wc);

    // Create Window
    hWnd = CreateWindowExW(0, pClassName, L"Happy Ocean Window", WS_SIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX, 1024, 1024, Window::width, Window::height, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, SW_SHOW);
}

void Window::UpdateEvents(D3D11& dx11, Camera& cam)
{
    //Getting previous Keyboard states
    GetKeyboardState(Input::prevKeyStates);

    //Getting previous width/height
    RECT rect;
    float width = 0;
    float height = 0;
    if (GetWindowRect(hWnd, &rect))
    {
        width = float(rect.right - rect.left);
        height = float(rect.bottom - rect.top);
    }

    // Hide Cursor
    if ((int)Cursor::cursorMode == 0)
        while (ShowCursor(false) >= 0);
    else
        while (ShowCursor(true) < 0);

    //Main message pump
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        // Handle Win32 Messages/Events
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //Checking if window has resized
    if (GetWindowRect(hWnd, &rect))
    {
        Events::width = rect.right - rect.left;
        Events::height = rect.bottom - rect.top;
        Events::size = std::sqrtf(float(Events::width * Events::width + Events::height * Events::height));
        if (Events::width != width || Events::height != height)
        {
            Events::Dispatch(EventType::WindowResize);
            Window::width = Events::width;
            Window::height = Events::height;

            cam.DestroyResources();

            dx11.OnResize(Events::width, Events::height);

            cam.OnResize();
        }
    }

    // Get cursor
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    Events::mouseDelta = Math::vec2(p.x - Events::mousePosition.x, p.y - Events::mousePosition.y);

    // Set cursor
    if (Cursor::cursorMode == CursorMode::Locked || Cursor::cursorMode == CursorMode::LockedAndHidden)
    {
        POINT point{ Window::GetWidth() / 2, Window::GetHeight() / 2 };
        ClientToScreen(hWnd, &point);
        SetCursorPos(point.x, point.y);
    }

    // Assign cursor Pos
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    Events::mousePosition = Math::vec2(p.x, p.y);
}

void Window::HandleEventBuffer()
{
    // Reset Events
    Events::Restore();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_MOVING:
        RECT rect;
        Events::Dispatch(EventType::WindowMoved);
        if (GetWindowRect(hWnd, &rect))
            Events::windowPosition = Math::vec2(rect.left, rect.top);
        break;
    case WM_CLOSE:
        Events::Dispatch(EventType::WindowClose);
        *UpdateApp = false;
        PostQuitMessage(1);
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
            Events::Dispatch(EventType::WindowFocus);
        else if (LOWORD(wParam) == WA_INACTIVE)
            Events::Dispatch(EventType::WindowLostFocus);
        break;
    case WM_KEYDOWN:
        Events::Dispatch(EventType::KeyPressed);
        Events::keyCode = (KeyCode)wParam;
        break;
    case WM_KEYUP:
        Events::Dispatch(EventType::KeyReleased);
        break;
    case WM_MOUSEMOVE:
        Events::Dispatch(EventType::MouseMoved);
        break;
    case WM_LBUTTONDOWN:
        Events::Dispatch(EventType::MouseButtonPressed);
        Events::button = Button::LButton;
        break;
    case WM_RBUTTONDOWN:
        Events::Dispatch(EventType::MouseButtonPressed);
        Events::button = Button::RButton;
        break;
    case WM_MBUTTONDOWN:
        Events::Dispatch(EventType::MouseButtonPressed);
        Events::button = Button::MButton;
        break;
    case WM_LBUTTONUP:
        Events::Dispatch(EventType::MouseButtonReleased);
        Events::button = Button::LButton;
        break;
    case WM_RBUTTONUP:
        Events::Dispatch(EventType::MouseButtonReleased);
        Events::button = Button::RButton;
        break;
    case WM_MBUTTONUP:
        Events::Dispatch(EventType::MouseButtonReleased);
        Events::button = Button::MButton;
        break;
    case WM_MOUSEWHEEL:
        Events::Dispatch(EventType::MouseScrolled);
        Events::mouseScrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
