#pragma once
#include "Object.h"

class Mesh : public Object
{
public:
	Mesh(const Vertex vertices[], const WORD indices[], size_t dataTypeSize, int vertexNumber, int indexSize);

	ComPtr<ID3D11Buffer> GetVertexBuffer() { return _vertexBuffer; }
	ComPtr<ID3D11Buffer> GetIndexBuffer() { return _indexBuffer; }

	UINT GetIndexCount() { return _indexCount; }


private:
	
	UINT _indexCount;
	
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

};

