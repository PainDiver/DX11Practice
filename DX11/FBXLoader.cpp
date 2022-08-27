#include "pch.h"
#include "FBXLoader.h"

void FBXLoader::AddScene(string path)
{
	FbxScene* scene = FbxScene::Create(_manager, "Scene");
	_importer = FbxImporter::Create(_manager, "Importer");
	bool status = _importer->Initialize(path.c_str());;

	_importer->Import(scene);
	_scene.push_back(scene);
	_importer->Destroy();

	_vertices.resize(_vertices.size() + 1);
	_indices.resize(_indices.size() + 1);

	ParseScene(_scene.size() - 1);
}



void FBXLoader::GetInputSet(int index, vector<Vertex>& outV, vector<WORD>& outI)
{
	for (const auto& fv : _vertices[index])
	{
		Vertex vertex;


		if (fv.rightHanded)
		{
			//right
			vertex.normal = {fv.normal.x,fv.normal.y,-fv.normal.z};
			vertex.tangent = { fv.tangent.x,fv.tangent.y,-fv.tangent.z };
			vertex.uv = { fv.uv.x ,-fv.uv.y };
			vertex.pos = { fv.pos.x / 10.f,fv.pos.y / 10.f,-fv.pos.z / 10.f };
		}
		else
		{
			//left
			vertex.normal = fv.normal;
			vertex.tangent = fv.tangent;
			vertex.uv = fv.uv;
			vertex.pos = { fv.pos.x/10.f,fv.pos.y/10.f,fv.pos.z / 10.f };
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

void FBXLoader::ParseScene(int index )
{
	FbxNode* rootNode = _scene[index]->GetRootNode();

	RecursiveParse(rootNode, index);
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

			int IndexinIndex = 0;
			for (int i = 0; i < indexCount; i++)
			{
				FIndex index;
				for (int j = 0; j < 3; j++)
				{
					int vertexIndex = mesh->GetPolygonVertex(i, j);
					ReadVertexInfo(mesh, vertexIndex,IndexinIndex, _vertices[containerIndex][vertexIndex]);
					IndexinIndex++;
				}
				_indices[containerIndex].push_back(index);
			}
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

void FBXLoader::ReadVertexInfo(FbxMesh* mesh, int controlIndex, int IndexinIndex,FVertex& vertex)
{
	FbxGeometryElementNormal* normal = mesh->GetElementNormal(0);
	//FbxGeometryElementBinormal* binormal = mesh->GetElementBinormal(0);
	FbxGeometryElementTangent* tangent = mesh->GetElementTangent(0);
	FbxGeometryElementUV* uv = mesh->GetElementUV(0);
	
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
		switch (uv->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = uv->GetDirectArray().GetAt(controlIndex);
				vertex.SetTex(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]));
			}
			else
			{
				int index = uv->GetIndexArray().GetAt(controlIndex);
				auto arr = uv->GetDirectArray().GetAt(index);
				vertex.SetTex(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]));
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				auto arr = uv->GetDirectArray().GetAt(IndexinIndex);
				vertex.SetTex(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]));
			}
			else
			{
				int index = uv->GetIndexArray().GetAt(IndexinIndex);
				auto arr = uv->GetDirectArray().GetAt(index);
				vertex.SetTex(static_cast<float>(arr.mData[0]), static_cast<float>(arr.mData[1]));
			}
			break;
		}
	}

}




