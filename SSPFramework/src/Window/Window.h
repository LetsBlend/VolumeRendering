#pragma once

class D3D11;
class Camera;

class Window
{
public:
    Window() {}
    ~Window() {}

    void StartUp(unsigned int width, unsigned int height, bool* UpdateApplication)
    {
        this->width = width;
        this->height = height;
        UpdateApp = UpdateApplication;
        Initialize();
    }

    void ShutDown()
    {

    }

    inline static int GetWidth() { return width; }
    inline static int GetHeight() { return height; }
    inline static HWND GetWindowHandle() { return hWnd; }

    void UpdateEvents(D3D11& dx11, Camera& cam);
    void HandleEventBuffer();

protected:
    void Initialize();

private:
    inline static bool* UpdateApp;
    inline static HWND hWnd;
    inline static unsigned int width;
    inline static unsigned int height;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};