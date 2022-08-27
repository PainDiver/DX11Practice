#pragma once


enum EStatus
{
	FAIL,
	SUCCESS
};

struct FVertex
{
	FVertex() {}
	void SetPos(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z;}	
	void SetTex(float x, float y) { uv.x = x, uv.y = y; }
	void SetNormal(float x, float y, float z) { normal.x = x, normal.y = y, normal.z = z; }
	void SetTangent(float x, float y, float z) { tangent.x = x, tangent.y = y, tangent.z = z; }

	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	
	bool rightHanded = true;
};


struct FIndex
{
	FIndex() {}

	array<int, 3> _indices;
};



class FBXLoader
{
private:
	FBXLoader() { _manager = FbxManager::Create(); }
public :
	static FBXLoader* GetInstance()
	{
		static FBXLoader instance;
		return &instance;
	}

	~FBXLoader()
	{
		_manager->Destroy();
	}

	void AddScene(string path);

	void GetInputSet(int index, vector<Vertex>& outV, vector<WORD>& outI);

private:
	void ParseScene(int index);

	void RecursiveParse(FbxNode* node,int index);
	
	void ReadVertexInfo(FbxMesh* mesh,int vertexIndex, int IndexinIndex, FVertex& vertex);


private:
	FbxManager* _manager;

	FbxImporter* _importer;

	// multiple scene values
	vector<FbxScene*> _scene;

	vector<vector<FVertex>> _vertices;

	vector<vector<FIndex>> _indices;
};

