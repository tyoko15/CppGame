#include "WinApp.h"
#include "DXDevice.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR lpCmdLine,
    int nCmdShow
)
{
    WinApp win;
    if (!win.Init(hInstance, nCmdShow)) return -1;

    DXDevice dx;
    // ウィンドウサイズは WinApp 側から取得して渡すのが良い
    uint32_t w = 1280, h = 720;
    if (!dx.Initialize(win.GetHwnd(), w, h)) {
        MessageBoxA(nullptr, "DX init failed", "Error", MB_OK);
        return -1;
    }

    while (win.IsRunning())
    {
        win.Message();

        // 毎フレームクリア（青）
        dx.BeginFrame(0.2f, 0.4f, 0.6f, 1.0f);

        // ★ 3D
        dx.RenderModel();

        // ★ 2D
        dx.DrawTextString(L"Hello DirectX 2.5D!", 50, 50);

        dx.EndFrame();
    }

    dx.Finalize();
    return 0;
}
