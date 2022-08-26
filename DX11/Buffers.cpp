#include "pch.h"
#include "Buffers.h"
#include "GameEngine.h"
#include "Device.h"


HRESULT Buffers::CreateBuffer(size_t size)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    ComPtr<ID3D11Buffer> bufferPointer;
    hr = DEVICE->CreateBuffer(&bd, NULL, bufferPointer.GetAddressOf());
    if (FAILED(hr))
    {
        Debug(hr,GWINDOW._hwnd);
        return hr;
    }
    _cbBuffers.push_back(bufferPointer);

}

void Buffers::UploadConstantBuffer(int index, void* address)
{
    CONTEXT->UpdateSubresource(_cbBuffers[index].Get(), 0, NULL, address, 0, 0);

}

void Buffers::Clear()
{
    _cbBuffers.clear();
}
