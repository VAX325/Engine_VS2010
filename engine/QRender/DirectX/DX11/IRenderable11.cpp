#include <Base_include.h>
#include "IRenderable11.h"

#pragma warning( push )
#pragma warning( disable : 4005)
#pragma warning( disable : 4838)
#include <D3DX11.h>
#pragma warning( pop )

IRenderable11::IRenderable11()
{
}

IRenderable11::IRenderable11(ID3D11Device* device, const char* Sprite, ID3D11DeviceContext* context)
{
    static const uint32_t s_pixel = 0xffc99aff;

    D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* tex;
    HRESULT hr = device->CreateTexture2D(&desc, &initData, &tex);

    if (SUCCEEDED(hr))
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
        SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = 1;

        hr = device->CreateShaderResourceView(tex, &SRVDesc, &texture);
    }

    if (FAILED(hr))
        GetLogManagerEx()->LogError("Can't create texture!", true);
}

IRenderable11::~IRenderable11()
{
}

void IRenderable11::Render(ID3D11DeviceContext* context)
{
    //context->DrawAuto();
}
