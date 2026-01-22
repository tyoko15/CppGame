#include "DXDevice.h"
#include <stdexcept>
#include <DirectXColors.h>

DXDevice::DXDevice() {}
DXDevice::~DXDevice() { Finalize(); }

bool DXDevice::Initialize(HWND hwnd, uint32_t width, uint32_t height)
{
    m_width = width; m_height = height;

    // スワップチェインの設定
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // デバイス作成
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL createdLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // Adapter (nullptr = default)
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,                    // Software driver
        0,                          // Flags (D3D11_CREATE_DEVICE_DEBUG を使うとデバッグ出力)
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &sd,
        &m_swapChain,
        &m_device,
        &createdLevel,
        &m_context
    );
    if (FAILED(hr)) return false;

    // バックバッファ取得 → RTV 作成
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
    if (FAILED(hr)) return false;

    // 深度ステンシルバッファ作成
    D3D11_TEXTURE2D_DESC descDepth{};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = m_device->CreateTexture2D(&descDepth, nullptr, &m_depthStencil);
    if (FAILED(hr)) return false;

    hr = m_device->CreateDepthStencilView(m_depthStencil.Get(), nullptr, &m_dsv);
    if (FAILED(hr)) return false;

    // RTV と DSV を OM にバインド
    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_dsv.Get());

    // ビューポート設定
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    m_viewport.Width = static_cast<float>(width);
    m_viewport.Height = static_cast<float>(height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &m_viewport);

    // 1. SpriteBatchの作成
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context.Get());

    // 2. SpriteFontの読み込み
    m_font = std::make_unique<DirectX::SpriteFont>(
        m_device.Get(),
        L"C:\\2DGame_Cpp\\2DGame_Cpp\\Assets\\Fonts\\Default.spritefont"
    );

    // 1. 共通ステートの作成
    m_states = std::make_unique<DirectX::CommonStates>(m_device.Get());

    // 2. エフェクトファクトリの作成（モデルの質感やテクスチャを管理）
    m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device.Get());

    // 3. モデルの読み込み
    try {
        m_model = DirectX::Model::CreateFromCMO(m_device.Get(), L"mono.cmo", *m_effectFactory);
    }
    catch (...) {
        // mono.cmo が見つからない場合の処理
        MessageBoxA(hwnd, "Model load failed!", "Error", MB_OK);
        return false;
    }

    // 4. カメラ行列の設定
    m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
        { 0.0f, 2.0f, 5.0f }, // カメラの位置
        { 0.0f, 0.0f, 0.0f }, // 見る点（原点）
        { 0.0f, 1.0f, 0.0f }  // 上方向
    );

    m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        DirectX::XM_PI / 4.f,
        static_cast<float>(width) / static_cast<float>(height),
        0.1f, 100.f
    );

    return true; // 初期化成功
}

void DXDevice::Finalize()
{
    if (m_context) m_context->ClearState();
    m_dsv.Reset();
    m_depthStencil.Reset();
    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
}

void DXDevice::BeginFrame(float r, float g, float b, float a)
{
    float color[4] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_rtv.Get(), color);
    m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXDevice::EndFrame()
{
    // 1 = VSync enabled (wait for vertical blank). 0 = no vsync.
    m_swapChain->Present(1, 0);
}


void DXDevice::DrawTextString(const wchar_t* text, float x, float y)
{
    if (!m_spriteBatch || !m_font) return;

    m_spriteBatch->Begin();

    DirectX::XMFLOAT2 pos(x, y);
    m_font->DrawString(
        m_spriteBatch.get(),
        text,
        pos,
        DirectX::Colors::White
    );

    m_spriteBatch->End();
}

void DXDevice::RenderModel()
{
    if (!m_model || !m_states) return;

    // 回転（時間で回す）
    m_world =
        DirectX::SimpleMath::Matrix::CreateRotationY(
            static_cast<float>(GetTickCount64()) * 0.001f
        );

    m_model->Draw(
        m_context.Get(),
        *m_states,
        m_world,
        m_view,
        m_projection
    );
}