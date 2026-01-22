#include "WinApp.h"
#include <windows.h>

WinApp::WinApp()
    : m_hWnd(nullptr), m_running(true)
{
}

WinApp::~WinApp()
{
}

bool WinApp::Init(HINSTANCE hInstance, int nCmdShow)
{
    // ウィンドウクラス設定
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"DXWindowClass";

    RegisterClassEx(&wc);

    // ウィンドウ作成
    m_hWnd = CreateWindowEx(
        0,
        L"DXWindowClass",
        L"My DirectX Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1920 / 2, 1080 / 2,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!m_hWnd) return false;

    ShowWindow(m_hWnd, nCmdShow);
    return true;
}

void WinApp::Message()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) {
            m_running = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool WinApp::IsRunning() const
{
    return m_running;
}

LRESULT CALLBACK WinApp::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}