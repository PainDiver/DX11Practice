#include "pch.h"
#include "Shader.h"
#include "Device.h"
#include "GameEngine.h"
#include "Device.h"

Shader::Shader(const WCHAR* vs, const WCHAR* ps, uint8 rasterizerDepthState)
{
    HRESULT hr = S_OK;

    CD3D11_DEPTH_STENCIL_DESC dsc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
    CD3D11_RASTERIZER_DESC rdsc =  CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
    CD3D11_BLEND_DESC bdsc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());;
   
    switch (rasterizerDepthState)
    {
    case RASTERIZER_DEPTH_DEFAULT:
        dsc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
        rdsc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rdsc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        
        break;
    case RASTERIZER_DEPTH_SKYMAP:
        dsc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
        rdsc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        rdsc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        break;
    case RASTERIZER_DEPTH_WIREFRAME:
        dsc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
        rdsc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rdsc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        break;
    case RASTERIZER_DEPTH_UI:
        dsc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
        rdsc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        rdsc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        break;    
    }


    hr = DEVICE->CreateDepthStencilState(&dsc, _currentDepthStencilState.GetAddressOf());
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

     hr = DEVICE->CreateRasterizerState(&rdsc, _currentRasterizerState.GetAddressOf());
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

     hr = DEVICE->CreateBlendState(&bdsc, _currentBlendState.GetAddressOf());
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);


    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = DEVICE->CreateSamplerState(&sampDesc, _samplerState.GetAddressOf());


    // Compile the vertex shader
    ComPtr<ID3DBlob> pVSBlob = NULL;
    hr = CompileShaderFromFile(vs, "VS", "vs_4_0", pVSBlob.GetAddressOf());
    if (FAILED(hr))
    {
        Debug(hr,GWINDOW._hwnd);
    }

    // Create the vertex shader
    hr = DEVICE->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(hr))
    {
        Debug(hr, GWINDOW._hwnd);
    }

    // Compile the pixel shader
    ComPtr<ID3DBlob> pPSBlob = NULL;
    hr = CompileShaderFromFile(ps, "PS", "ps_4_0", pPSBlob.GetAddressOf());
    if (FAILED(hr))
    {
        Debug(hr, GWINDOW._hwnd);
    }

    // Create the pixel shader
    hr = DEVICE->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);


    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = DEVICE->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), _inputLayout.GetAddressOf());

    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

}


HRESULT Shader::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> pErrorBlob;
    hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, pErrorBlob.GetAddressOf());
    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;

}