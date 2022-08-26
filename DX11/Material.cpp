#include "pch.h"
#include "Material.h"
#include "GameEngine.h"
#include "Device.h"

Material::Material(const WCHAR* path)
{
    HRESULT hr;

    ScratchImage image;
    hr = LoadFromWICFile(path, WIC_FLAGS_NONE, nullptr, image);
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

    hr = CreateShaderResourceView(DEVICE.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture.GetAddressOf());
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

  

}
