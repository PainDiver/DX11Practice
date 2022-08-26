#pragma once
class Buffers
{
public:

	HRESULT CreateBuffer(size_t size);

	vector<ComPtr<ID3D11Buffer>> GetCB() { return _cbBuffers; }

	void UploadConstantBuffer(int index, void* address);

	void Clear();

private:

	vector< ComPtr<ID3D11Buffer>> _cbBuffers;
};
