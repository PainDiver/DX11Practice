#include "pch.h"
#include "Renderer.h"
#include "GameEngine.h"
#include "Camera.h"
#include "Device.h"
#include "Transform.h"
#include "Buffers.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Shader.h"
#include "GameObject.h"

void Renderer::ShadowRender()
{
    switch (_renderType)
    {
    case ERenderType::DEFAULT:
        if (Camera::CheckFrustum(GetTransform()->GetPos(), GetTransform()->GetWidth()) == false)
            return;
        RenderDefault();
        RenderEnd();
        break;

    case ERenderType::INSTANCE:
        RenderInstance();
        RenderEnd();
        break;
    }
}



void Renderer::Render()
{
    
    switch (_renderType)
    {
    case ERenderType::DEFAULT:
        if (Camera::CheckFrustum(GetTransform()->GetPos(), GetTransform()->GetWidth()) == false)
            return;
        RenderDefault();
        RenderEnd();
        break;

    case ERenderType::INSTANCE:
        
        RenderInstance();
        RenderEnd();
        break;

    case ERenderType::SKYMAP:
        RenderSkyMap();
        RenderEnd();
        break;
    case ERenderType::UI:
        RenderUI();
        RenderEnd();
        break;
    }
}


void Renderer::RenderDefault()
{
    CONTEXT->OMSetDepthStencilState(_shader->GetDepthStencilState().Get(), 0);

    CONTEXT->RSSetState(_shader->GetRasterizerState().Get());


    CONSTANTBUFFER->CreateBuffer(sizeof(MatInfo));
    CONSTANTBUFFER->CreateBuffer(sizeof(LightInfo));
    CONSTANTBUFFER->CreateBuffer(sizeof(Flags));

    LightInfo light;
    light.color = Light::GetColor();
    light.dir = Light::GetDirection();

    MatInfo mat;
    mat.world = XMMatrixTranspose(GetTransform()->GetWorldMat());
    mat.view = XMMatrixTranspose(Camera::GetViewMat());
    mat.proj = XMMatrixTranspose(Camera::GetProjMat(CameraType::PERSPECTIVE));
    mat.WVP = mat.proj * mat.view * mat.world ;

    Flags flag;
    flag.instancing = 0;

    CONSTANTBUFFER->UploadConstantBuffer(0, &mat);
    CONSTANTBUFFER->UploadConstantBuffer(1, &light);
    CONSTANTBUFFER->UploadConstantBuffer(2, &flag);

}


void Renderer::RenderInstance()
{

    CONTEXT->OMSetDepthStencilState(_shader->GetDepthStencilState().Get(), 0);

    CONTEXT->RSSetState(_shader->GetRasterizerState().Get());

    CONSTANTBUFFER->CreateBuffer(sizeof(MatInfo));
    CONSTANTBUFFER->CreateBuffer(sizeof(LightInfo));
    CONSTANTBUFFER->CreateBuffer(sizeof(Flags));
    CONSTANTBUFFER->CreateBuffer(sizeof(InstancingInfo));


    MatInfo mat;
    mat.world = XMMatrixTranspose(GetTransform()->GetWorldMat());
    mat.view = XMMatrixTranspose(Camera::GetViewMat());
    mat.proj = XMMatrixTranspose(Camera::GetProjMat(CameraType::PERSPECTIVE));
    mat.WVP =  mat.proj * mat.view * mat.world;

    LightInfo light;
    light.color = Light::GetColor();
    light.dir = Light::GetDirection();

    Flags flag;
    flag.instancing = 1;

    InstancingInfo instancing;
    
    XMFLOAT3 pos = GetTransform()->GetPos();
    XMFLOAT3 rot = GetTransform()->GetRotation();
    XMFLOAT3 scale = GetTransform()->GetScale();

    int j = 0;
    for (int i = 0; i < _instancingNumber; i++)
    {
        if (i % 20 == 0)
            j++;
        instancing.instanceWorld[i] = XMMatrixTranspose(MakeSRT({ i%20 * 5.f+pos.x,0.f+pos.y, j * 5.f+ pos.z }, { rot.x,rot.y,rot.z }, { scale.x,scale.y,scale.z }));
    }

    CONSTANTBUFFER->UploadConstantBuffer(0, &mat);
    CONSTANTBUFFER->UploadConstantBuffer(1, &light);
    CONSTANTBUFFER->UploadConstantBuffer(2, &flag);
    CONSTANTBUFFER->UploadConstantBuffer(3, &instancing);
    
}

void Renderer::RenderSkyMap()
{
    CONTEXT->OMSetDepthStencilState(_shader->GetDepthStencilState().Get(), 0);

    CONTEXT->RSSetState(_shader->GetRasterizerState().Get());

    CONSTANTBUFFER->CreateBuffer(sizeof(MatInfo));

    MatInfo mat;
    mat.world = XMMatrixTranspose(GetTransform()->GetWorldMat());
    mat.view = XMMatrixTranspose(Camera::GetViewMat());
    mat.proj = XMMatrixTranspose(Camera::GetProjMat(CameraType::PERSPECTIVE));
    mat.WVP = mat.proj * mat.view * mat.world;

    CONSTANTBUFFER->UploadConstantBuffer(0, &mat);

    
}

void Renderer::RenderUI()
{

    CONTEXT->OMSetDepthStencilState(_shader->GetDepthStencilState().Get(), 0);

    CONTEXT->RSSetState(_shader->GetRasterizerState().Get());

    CONSTANTBUFFER->CreateBuffer(sizeof(MatInfo));

    MatInfo mat;
    mat.world = XMMatrixTranspose(GetTransform()->GetWorldMat());
    mat.view = (XMMatrixIdentity());
    mat.proj = (Camera::GetProjMat(CameraType::ORTHOGRAPHIC));
    mat.WVP =  mat.world * mat.view* mat.proj;

    CONSTANTBUFFER->UploadConstantBuffer(0, &mat);

}


void Renderer::RenderEnd()
{
    // Set primitive topology
    CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the input layout
    CONTEXT->IASetInputLayout(_shader->GetInputLayout().Get());

    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ComPtr < ID3D11Buffer> vertexBuffer = _mesh->GetVertexBuffer();
    CONTEXT->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    // Set index buffer
    CONTEXT->IASetIndexBuffer(_mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

    CONTEXT->VSSetShader(_shader->GetVertexShader().Get(), NULL, 0);


    vector< ComPtr<ID3D11Buffer>> cbuffers = CONSTANTBUFFER->GetCB();
    for (int i = 0; i < cbuffers.size(); i++)
    {
        CONTEXT->VSSetConstantBuffers(i, 1, cbuffers[i].GetAddressOf());
        CONTEXT->PSSetConstantBuffers(i, 1, cbuffers[i].GetAddressOf());
    }

    CONTEXT->PSSetShader(_shader->GetPixelShader().Get(), NULL, 0);
    for (int i = 0; i < _material.size(); i++)
    {
        ComPtr < ID3D11ShaderResourceView> textureView = _material[i]->GetTexture();
        CONTEXT->PSSetShaderResources(i, 1, textureView.GetAddressOf());
    }
    ComPtr<ID3D11SamplerState> samplerState = _shader->GetSamplerState();

    CONTEXT->PSSetSamplers(0, 1, samplerState.GetAddressOf());


    if (_instancingNumber > 1)
    {
        CONTEXT->DrawIndexedInstanced(_mesh->GetIndexCount(), _instancingNumber, 0, 0, 0);
    }
    else
    {
        CONTEXT->DrawIndexed(_mesh->GetIndexCount(), 0, 0);
    }

    for (int i = 0; i < _material.size(); i++)
    {
        ComPtr<ID3D11ShaderResourceView> pSRV = NULL;
        CONTEXT->PSSetShaderResources(i, 1, pSRV.GetAddressOf());
    }

    CONSTANTBUFFER->Clear();
}

