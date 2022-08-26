#include "pch.h"
#include "Mesh.h"
#include "GameEngine.h"

Mesh::Mesh(const Vertex vertices[], const WORD indices[],size_t dataTypeSize, int vertexNumber,int indexSize)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = dataTypeSize * vertexNumber;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = DEVICE->CreateBuffer(&bd, &InitData, &_vertexBuffer);
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indexSize;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = DEVICE->CreateBuffer(&bd, &InitData, &_indexBuffer);
    if (FAILED(hr))
        Debug(hr, GWINDOW._hwnd);

    _indexCount = indexSize;


}
