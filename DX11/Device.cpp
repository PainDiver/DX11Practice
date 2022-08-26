#include "pch.h"
#include "Device.h"
#include "GameEngine.h"

HRESULT Device::Init(const GWindow& window)
{
    HRESULT hr = S_OK;


    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_DRIVER_TYPE   g_driverType = D3D_DRIVER_TYPE_HARDWARE;

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
    
    D3D_FEATURE_LEVEL featureLV = D3D_FEATURE_LEVEL_11_0;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = window._width;
    sd.BufferDesc.Height = window._height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window._hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &sd, _swapChain.GetAddressOf(), _device.GetAddressOf(), &featureLV, _context.GetAddressOf());
        if (SUCCEEDED(hr))
            break;
    }
    
    ComPtr<ID3D11Texture2D> pBackBuffer = NULL;
    hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = _device->CreateRenderTargetView(pBackBuffer.Get(), NULL, _renderTargetView.GetAddressOf());
    if (FAILED(hr))
        return hr;

//////MULTIPLE RENDERTARGET///////////////////////////////////////////////////
    CD3D11_TEXTURE2D_DESC tdsc;
    ZeroMemory(&tdsc, sizeof(D3D11_TEXTURE2D_DESC));
    tdsc.Width = GWINDOW._width;
    tdsc.Height = GWINDOW._height;
    tdsc.MipLevels = 1;
    tdsc.ArraySize = 1;
    tdsc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    tdsc.SampleDesc.Count = 1;
    tdsc.Usage = D3D11_USAGE_DEFAULT;
    tdsc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    tdsc.CPUAccessFlags = 0;
    tdsc.MiscFlags = 0;
    
    CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = tdsc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    renderTargetViewDesc.Buffer.NumElements = 1;

    _device->CreateTexture2D(&tdsc, NULL, _multipleRenderTarget.GetAddressOf());
    _device->CreateRenderTargetView(_multipleRenderTarget.Get(), &renderTargetViewDesc, _multipleRenderTargetView.GetAddressOf());    


    CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(CD3D11_SHADER_RESOURCE_VIEW_DESC));
    srvd.Format = tdsc.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MostDetailedMip = 0;
    srvd.Texture2D.MipLevels = 1;

    // Create the shader resource view.
    DEVICE->CreateShaderResourceView(_multipleRenderTarget.Get(), &srvd, _multipleRenderTargetSRV.GetAddressOf());
    
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
    desc.Width = window._width;
    desc.Height = window._height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    hr = _device->CreateTexture2D(&desc, NULL, _multipleDepthStencil.GetAddressOf());


    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsv;;
    ZeroMemory(&dsv, sizeof(dsv));
    dsv.Format = desc.Format;
    dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsv.Texture2D.MipSlice = 0;


    hr = _device->CreateDepthStencilView(_multipleDepthStencil.Get(), &dsv, _multipleRenderDepthStencilView.GetAddressOf());
    if (FAILED(hr))
        return hr;


//////////////////////////////////////////////////////////////////////
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
    descDepth.Width = window._width;
    descDepth.Height = window._height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = _device->CreateTexture2D(&descDepth, NULL, _depthStencil.GetAddressOf());
    if (FAILED(hr))
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = _device->CreateDepthStencilView(_depthStencil.Get(), &descDSV, _depthStencilView.GetAddressOf());
    if (FAILED(hr))
        return hr;


}
