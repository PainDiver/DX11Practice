#include "pch.h"
#include "FBXLoader.h"


void FBXLoader::AddScene(string path)
{
	FbxScene* scene = FbxScene::Create(_manager, "Scene");
	_importer = FbxImporter::Create(_manager, "Importer");
	bool status = _importer->Initialize(path.c_str());;


	scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	_importer->Import(scene);
	_scene.push_back(scene);
	_importer->Destroy();

	_vertices.resize(_vertices.size() + 1);
	_indices.resize(_indices.size() + 1);
	_animClips.resize(_animClips.size() + 1);
	_bones.resize(_bones.size() + 1);

	LoadAnimationInfo(_scene.size()-1);
	LoadBones(scene->GetRootNode(),_scene.size()-1);


	ParseScene(_scene.size() - 1);
	int a=0;
	a++;
}



void FBXLoader::GetInputSet(int index, vector<Vertex>& outV, vector<WORD>& outI)
{
	for (int i=0;i<_vertices[index].size();i++)
	{
		FVertex fv = _vertices[index][i];
		Vertex vertex;


		if (fv.rightHanded)
		{
			//right
			vertex.normal = {fv.normal.x,fv.normal.y,-fv.normal.z};
			vertex.tangent = { fv.tangent.x,fv.tangent.y,-fv.tangent.z };
			vertex.uv = fv.uv;
			vertex.pos = { fv.pos.x ,fv.pos.y,-fv.pos.z };
		}
		else
		{
			//left
			vertex.normal = fv.normal;
			vertex.tangent = fv.tangent;
			vertex.uv = fv.uv;
			vertex.pos = { fv.pos.x,fv.pos.y,fv.pos.z};
		}

		outV.push_back(vertex);
	}

	for (const auto& indices : _indices[index])
	{
		outI.push_back(indices._indices[0]);
		outI.push_back(indices._indices[1]);
		outI.push_back(indices._indices[2]);
	}
	
}

void FBXLoader::ParseScene(int index)
{
	FbxNode* rootNode = _scene[index]->GetRootNode();

	RecursiveParse(rootNode,index);
}


void FBXLoader::RecursiveParse(FbxNode* node,int containerIndex)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute == nullptr)
	{
		goto PASS;
	}

	switch (nodeAttribute->GetAttributeType())
	{
		case FbxNodeAttribute::eMesh :
		{
			FbxMesh* mesh = node->GetMesh();
			if (mesh->GetElementTangentCount() == 0)
			{
				mesh->GenerateTangentsDataForAllUVSets();
			}
			bool ccw = mesh->CheckIfVertexNormalsCCW();

			int indexCount = mesh->GetPolygonCount();
			
			int vertexCount = mesh->GetControlPointsCount();
			for (int i = 0; i < vertexCount; i++)
			{
				FVertex vertex;
				FbxVector4 pos = mesh->GetControlPointAt(i);
				vertex.SetPos(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));
				vertex.rightHanded = ccw;
				_vertices[containerIndex].push_back(vertex);
			}

			int IndexInIndex = 0;
			for (int i = 0; i < indexCount; i++)
			{
				FIndex index;
				for (int j = 0; j < 3; j++)
				{
					int vertexIndex = mesh->GetPolygonVertex(i, j);
					int vertexInIndex = mesh->GetPolygonVertexIndex(vertexIndex);
					
					if (ccw)
					{
						index._indices[2 - j] = vertexIndex;
					}
					else
					{
						index._indices[j] = vertexIndex;
					}

					int texIndex = mesh->GetTextureUVIndex(i,j);
					ReadVertexInfo(mesh, vertexIndex,IndexInIndex, containerIndex, texIndex);
					
					IndexInIndex++;

				}
				_indices[containerIndex].push_back(index);
			}

			//LoadAnimationData(mesh, &meshInfo);

			break;
		}
	}

	
PASS:
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		RecursiveParse(node->GetChild(i), containerIndex);
	}
}


void FBXLoader::ReadVertexInfo(FbxMesh* mesh, int controlIndex, int IndexinIndex,int containerIndex,int texIndex)
{
	FbxGeometryElementNormal* normal = mesh->GetElementNormal(0);
	//FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal(0);
	FbxGeometryElementTangent* tangent = mesh->GetElementTangent(0);
	FbxGeometryElementUV* uv = mesh->GetElementUV(0);
	
	FVertex& vertex = _vertices[containerIndex][controlIndex];


	if (mesh->GetElementNormalCount() > 0)
	{
		switch (normal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = normal->GetDirectArray().GetAt(controlIndex);
				vertex.SetNormal(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			else
			{
				int index = normal->GetIndexArray().GetAt(controlIndex);
				auto arr = normal->GetDirectArray().GetAt(index);
				vertex.SetNormal(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
		
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = normal->GetDirectArray().GetAt(IndexinIndex);
				vertex.SetNormal(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));

			}
			else
			{
				int index = normal->GetIndexArray().GetAt(IndexinIndex);
				auto arr = normal->GetDirectArray().GetAt(index);
				vertex.SetNormal(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));

			}
			break;
		}
	}

	//if (mesh->GetElementBinormalCount())
	//{
	//	switch (binormal->GetMappingMode())
	//	{
	//	case FbxGeometryElement::eByControlPoint:
	//		if (binormal->GetReferenceMode() == FbxGeometryElement::eDirect)
	//		{
	//			auto arr = binormal->GetDirectArray().GetAt(controlIndex);
	//			vertex._binormal[0] = static_cast<float>(arr.mData[0]);
	//			vertex._binormal[1] = static_cast<float>(arr.mData[1]);
	//			vertex._binormal[2] = static_cast<float>(arr.mData[2]);
	//		}
	//		else
	//		{
	//			int index = binormal->GetIndexArray().GetAt(controlIndex);
	//			auto arr = binormal->GetDirectArray().GetAt(index);
	//			vertex._binormal[0] = static_cast<float>(arr.mData[0]);
	//			vertex._binormal[1]  = static_cast<float>(arr.mData[1]);
	//			vertex._binormal[2]  = static_cast<float>(arr.mData[2]);
	//		}
	//		break;

	//	case FbxGeometryElement::eByPolygonVertex:
	//		if (binormal->GetReferenceMode() == FbxGeometryElement::eDirect)
	//		{
	//			auto arr = binormal->GetDirectArray().GetAt(IndexinIndex);
	//			vertex._binormal[0]  = static_cast<float>(arr.mData[0]);
	//			vertex._binormal[1]  = static_cast<float>(arr.mData[1]);
	//			vertex._binormal[2]  = static_cast<float>(arr.mData[2]);
	//		}
	//		else
	//		{
	//			int index = binormal->GetIndexArray().GetAt(IndexinIndex);
	//			auto arr = binormal->GetDirectArray().GetAt(index);
	//			vertex._binormal[0] = static_cast<float>(arr.mData[0]);
	//			vertex._binormal[1] = static_cast<float>(arr.mData[1]);
	//			vertex._binormal[2] = static_cast<float>(arr.mData[2]);
	//		}
	//		break;
	//	}
	//}

	if (mesh->GetElementTangentCount())
	{
		switch (tangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = tangent->GetDirectArray().GetAt(controlIndex);
				vertex.SetTangent(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			else
			{
				int index = tangent->GetIndexArray().GetAt(controlIndex);
				auto arr = tangent->GetDirectArray().GetAt(index);
				vertex.SetTangent(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = tangent->GetDirectArray().GetAt(IndexinIndex);
				vertex.SetTangent(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			else
			{
				int index = tangent->GetIndexArray().GetAt(IndexinIndex);
				auto arr = tangent->GetDirectArray().GetAt(index);
				vertex.SetTangent(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]), static_cast<float>(arr.mData[2]));
			}
			break;
		}
	}


	if (mesh->GetElementUVCount())
	{
		auto arr = uv->GetDirectArray().GetAt(texIndex);
		vertex.SetTex(static_cast<float>(arr.mData[0]), static_cast<float>(1-arr.mData[1]));
	}

}

void FBXLoader::LoadBones(FbxNode* node,int containerIndex, int32 idx, int32 parentIdx)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		FbxBoneInfo bone{};
		bone.boneName = (node->GetName());
		bone.parentIndex = parentIdx;
		_bones[containerIndex].push_back(bone);
	}

	const int32 childCount = node->GetChildCount();
	for (int32 i = 0; i < childCount; i++)
		LoadBones(node->GetChild(i), containerIndex, static_cast<int32>(_bones[containerIndex].size()), idx);
}

void FBXLoader::LoadAnimationInfo(int containerIndex)
{
	_scene[containerIndex]->FillAnimStackNameArray(OUT _animNames);

	const int32 animCount = _animNames.GetCount();
	for (int32 i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = _scene[containerIndex]->FindMember<FbxAnimStack>(_animNames[i]->Buffer());
		if (animStack == nullptr)
			continue;

		FbxAnimClipInfo animClip{};
		animClip.name = animStack->GetName();
		animClip.keyFrames.resize(_bones[containerIndex].size()); // Ű�������� ���� ������ŭ

		FbxTakeInfo* takeInfo = _scene[containerIndex]->GetTakeInfo(animStack->GetName());
		animClip.startTime = takeInfo->mLocalTimeSpan.GetStart();
		animClip.endTime = takeInfo->mLocalTimeSpan.GetStop();
		animClip.mode = _scene[containerIndex]->GetGlobalSettings().GetTimeMode();

		_animClips[containerIndex].push_back(animClip);
	}
}










//void FBXLoader::LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo)
//{
//	const int32 skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
//	if (skinCount <= 0 || _animClips.empty())
//		return;
//
//	meshInfo->hasAnimation = true;
//
//	for (int32 i = 0; i < skinCount; i++)
//	{
//		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));
//
//		if (fbxSkin)
//		{
//			FbxSkin::EType type = fbxSkin->GetSkinningType();
//			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
//			{
//				const int32 clusterCount = fbxSkin->GetClusterCount();
//				for (int32 j = 0; j < clusterCount; j++)
//				{
//					FbxCluster* cluster = fbxSkin->GetCluster(j);
//					if (cluster->GetLink() == nullptr)
//						continue;
//
//					int32 boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
//					assert(boneIdx >= 0);
//
//					FbxAMatrix matNodeTransform = GetTransform(mesh->GetNode());
//					LoadBoneWeight(cluster, boneIdx, meshInfo);
//					LoadOffsetMatrix(cluster, matNodeTransform, boneIdx, meshInfo);
//
//					const int32 animCount = _animNames.Size();
//					for (int32 k = 0; k < animCount; k++)
//						LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
//				}
//			}
//		}
//	}
//
//	FillBoneWeight(mesh, meshInfo);
//}
//
//
//
//void FBXLoader::FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo)
//{
//	const int32 size = static_cast<int32>(meshInfo->boneWeights.size());
//	for (int32 v = 0; v < size; v++)
//	{
//		BoneWeight& boneWeight = meshInfo->boneWeights[v];
//		boneWeight.Normalize();
//
//		float animBoneIndex[4] = {};
//		float animBoneWeight[4] = {};
//
//		const int32 weightCount = static_cast<int32>(boneWeight.boneWeights.size());
//		for (int32 w = 0; w < weightCount; w++)
//		{
//			animBoneIndex[w] = static_cast<float>(boneWeight.boneWeights[w].first);
//			animBoneWeight[w] = static_cast<float>(boneWeight.boneWeights[w].second);
//		}
//
//		memcpy(&meshInfo->vertices[v].indices, animBoneIndex, sizeof(Vec4));
//		memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(Vec4));
//	}
//}
//
//
//
//
//
//
//
//void FBXLoader::LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo)
//{
//	const int32 indicesCount = cluster->GetControlPointIndicesCount();
//	for (int32 i = 0; i < indicesCount; i++)
//	{
//		double weight = cluster->GetControlPointWeights()[i];
//		int32 vtxIdx = cluster->GetControlPointIndices()[i];
//		meshInfo->boneWeights[vtxIdx].AddWeights(boneIdx, weight);
//	}
//}

//void FBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo)
//{
//	FbxAMatrix matClusterTrans;
//	FbxAMatrix matClusterLinkTrans;
//	// The transformation of the mesh at binding time 
//	cluster->GetTransformMatrix(matClusterTrans);
//	// The transformation of the cluster(joint) at binding time from joint space to world space 
//	cluster->GetTransformLinkMatrix(matClusterLinkTrans);
//
//	FbxVector4 V0 = { 1, 0, 0, 0 };
//	FbxVector4 V1 = { 0, 0, 1, 0 };
//	FbxVector4 V2 = { 0, 1, 0, 0 };
//	FbxVector4 V3 = { 0, 0, 0, 1 };
//
//	FbxAMatrix matReflect;
//	matReflect[0] = V0;
//	matReflect[1] = V1;
//	matReflect[2] = V2;
//	matReflect[3] = V3;
//
//	FbxAMatrix matOffset;
//	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans;
//	matOffset = matReflect * matOffset * matReflect;
//
//	_bones[boneIdx]->matOffset = matOffset.Transpose();
//}
//
//void FBXLoader::LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo)
//{
//	if (_animClips.empty())
//		return;
//
//	FbxVector4	v1 = { 1, 0, 0, 0 };
//	FbxVector4	v2 = { 0, 0, 1, 0 };
//	FbxVector4	v3 = { 0, 1, 0, 0 };
//	FbxVector4	v4 = { 0, 0, 0, 1 };
//	FbxAMatrix	matReflect;
//	matReflect.mData[0] = v1;
//	matReflect.mData[1] = v2;
//	matReflect.mData[2] = v3;
//	matReflect.mData[3] = v4;
//
//	FbxTime::EMode timeMode = _scene->GetGlobalSettings().GetTimeMode();
//
//	// �ִϸ��̼� �����
//	FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[animIndex]->Buffer());
//	_scene->SetCurrentAnimationStack(OUT animStack);
//
//	FbxLongLong startFrame = _animClips[animIndex]->startTime.GetFrameCount(timeMode);
//	FbxLongLong endFrame = _animClips[animIndex]->endTime.GetFrameCount(timeMode);
//
//	for (FbxLongLong frame = startFrame; frame < endFrame; frame++)
//	{
//		FbxKeyFrameInfo keyFrameInfo = {};
//		FbxTime fbxTime = 0;
//
//		fbxTime.SetFrame(frame, timeMode);
//
//		FbxAMatrix matFromNode = node->EvaluateGlobalTransform(fbxTime);
//		FbxAMatrix matTransform = matFromNode.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
//		matTransform = matReflect * matTransform * matReflect;
//
//		keyFrameInfo.time = fbxTime.GetSecondDouble();
//		keyFrameInfo.matTransform = matTransform;
//
//		_animClips[animIndex]->keyFrames[boneIdx].push_back(keyFrameInfo);
//	}
//}
//
//int32 FBXLoader::FindBoneIndex(string name)
//{
//	wstring boneName = wstring(name.begin(), name.end());
//
//	for (UINT i = 0; i < _bones.size(); ++i)
//	{
//		if (_bones[i]->boneName == boneName)
//			return i;
//	}
//
//	return -1;
//}
//
//FbxAMatrix FBXLoader::GetTransform(FbxNode* node)
//{
//	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
//	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
//	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
//	return FbxAMatrix(translation, rotation, scaling);
//}
