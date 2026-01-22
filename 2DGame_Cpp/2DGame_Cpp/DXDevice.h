#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <cstdint>
#include <memory>

#include <SimpleMath.h>

#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "Model.h"
#include "Effects.h"
#include "CommonStates.h"

class DXDevice
{
public:
    DXDevice();
    ~DXDevice();

    bool Initialize(HWND hwnd, uint32_t width, uint32_t height);
    void Finalize();

    void BeginFrame(float r, float g, float b, float a);
    void EndFrame();

    void DrawTextString(const wchar_t* text, float x, float y);

    // Åö Ç±ÇÍÇïKÇ∏êÈåæÇ∑ÇÈ
    void RenderModel();

private:
    // --- DirectX äÓñ{ ---
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_depthStencil;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
    D3D11_VIEWPORT                                 m_viewport{};

    uint32_t m_width = 0;
    uint32_t m_height = 0;

    // --- 2D ---
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont>  m_font;

    // --- 3D ---
    std::unique_ptr<DirectX::Model>        m_model;
    std::unique_ptr<DirectX::CommonStates> m_states;
    std::unique_ptr<DirectX::EffectFactory> m_effectFactory;

    DirectX::SimpleMath::Matrix m_world;
    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_projection;
};
