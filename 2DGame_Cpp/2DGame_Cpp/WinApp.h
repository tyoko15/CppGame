#pragma once
#include <windows.h>

class WinApp
{
public:
    WinApp();
    ~WinApp();

    bool Init(HINSTANCE hInstance, int nCmdShow); // ウィンドウ作成
    void Message();                               // メッセージ処理
    bool IsRunning() const;                       // ゲームループ継続チェック
    HWND GetHwnd() const { return m_hWnd; }

private:
    HWND m_hWnd;
    bool m_running;

    // ウィンドウプロシージャ（イベントを処理する関数）
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
