#pragma once


enum EStatus
{
	FAIL,
	SUCCESS
};

struct FbxBoneInfo
{
	string					boneName;
	int32					parentIndex;
	FbxAMatrix				matOffset;
};

struct FVertex
{
	FVertex() {}
	void SetPos(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z;}	
	void SetTex(float x, float y) { uv.x = x, uv.y = y; }
	void SetNormal(float x, float y, float z) { normal.x = x, normal.y = y, normal.z = z; }
	void SetTangent(float x, float y, float z) { tangent.x = x, tangent.y = y, tangent.z = z; }

	XMFLOAT3 pos;
	XMFLOAT2 uv = {0,0};
	XMFLOAT3 normal;
	XMFLOAT3 tangent;

	bool rightHanded;

};


struct FIndex
{
	FIndex() {}

	array<int, 3> _indices;
};

struct FbxKeyFrameInfo
{
	FbxAMatrix  matTransform;
	double		time;
};

struct FbxAnimClipInfo
{
	string			name;
	FbxTime			startTime;
	FbxTime			endTime;
	FbxTime::EMode	mode;
	vector<vector<FbxKeyFrameInfo>>	keyFrames;
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

	void RecursiveParse(FbxNode* node,int containerIndex);

	void ReadVertexInfo(FbxMesh* mesh,int vertexIndex, int IndexinIndex, int containerIndex,int texIndex);


	void LoadBones(FbxNode* node,int containerIndex, int32 idx = 0, int32 parentIdx = -1);

	void LoadAnimationInfo(int containerIndex);




//	void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
//	void LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo);
//	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo);
//	void LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* container);

//	int32 FindBoneIndex(string name);
//	FbxAMatrix GetTransform(FbxNode* node);

//	void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

private:

	FbxManager* _manager;

	FbxImporter* _importer;

	// multiple scene values
	vector<FbxScene*> _scene;

	vector<vector<FVertex>> _vertices;

	vector<vector<FIndex>> _indices;

	vector<vector<FbxBoneInfo>> _bones;

	FbxArray<FbxString*>		_animNames;

	vector<vector<FbxAnimClipInfo>>	_animClips;
};

