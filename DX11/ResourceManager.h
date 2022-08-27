#pragma once
#include "GameObject.h"
#include "Object.h"


namespace RectSpace
{
    static const Vertex vertices[] =
    {
        //윗면
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f),  XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f),   XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f),    XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f),   XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
              
        //아랫면
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.f,-1.f,0.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f),  XMFLOAT3(0.f,-1.f,0.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f),   XMFLOAT3(0.f,-1.f,0.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f),  XMFLOAT3(0.f,-1.f,0.f),XMFLOAT3(-1.f,0.f,0.f) },
                                                   
        //왼쪽면
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f),  XMFLOAT3(-1.0f, 0.f, 0.f),XMFLOAT3(0.f,0.f,-1.f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.f, 0.f),XMFLOAT3(0.f,0.f,-1.f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f),  XMFLOAT3(-1.0f, 0.f, 0.f),XMFLOAT3(0.f,0.f,-1.f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f),   XMFLOAT3(-1.0f, 0.f, 0.f),XMFLOAT3(0.f,0.f,-1.f) },
                
        //오른쪽면
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f),   XMFLOAT3(1.0f,0.f,0.f),XMFLOAT3(0.f,0.f,1.f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f),  XMFLOAT3(1.0f,0.f,0.f),XMFLOAT3(0.f,0.f,1.f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f),   XMFLOAT3(1.0f,0.f,0.f),XMFLOAT3(0.f,0.f,1.f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f),    XMFLOAT3(1.0f,0.f,0.f),XMFLOAT3(0.f,0.f,1.f) },
                
        //앞면
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.f,0.0f,-1.f),XMFLOAT3(1.0f,0.f,0.f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f),  XMFLOAT3(0.f,0.f,-1.f),XMFLOAT3(1.0f,0.f,0.f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f),   XMFLOAT3(0.f,0.f,-1.f),XMFLOAT3(1.0f,0.f,0.f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f),  XMFLOAT3(0.f,0.f,-1.f),XMFLOAT3(1.0f,0.f,0.f) },
              
        //뒷면
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f),  XMFLOAT3(0.f,0.f,1.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f),   XMFLOAT3(0.f,0.f,1.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f),    XMFLOAT3(0.f,0.f,1.f),XMFLOAT3(-1.f,0.f,0.f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f),   XMFLOAT3(0.f,0.f,1.f),XMFLOAT3(-1.f,0.f,0.f) },
    };


    static const WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    static const int vertexSize = sizeof(vertices) / sizeof(Vertex);
    static const int indexSize = sizeof(indices) / sizeof(WORD);
}

namespace PlaneSpace
{
    static const Vertex vertices[] =
    {
        
        { XMFLOAT3(-1.0f, -1.0f, 0.f), XMFLOAT2(0.0f, 1.0f),  XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(-1.0f, 1.0f, 0.f), XMFLOAT2(0.0f, 0.0f),   XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, 1.0f, 0.f), XMFLOAT2(1.0f, 0.0f),    XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },
        { XMFLOAT3(1.0f, -1.0f, 0.f), XMFLOAT2(1.0f, 1.0f),   XMFLOAT3(0.f,1.f,0.f),XMFLOAT3(1.f,0.f,0.f) },

    };

    static const WORD indices[] =
    {
        0,1,2,
        0,2,3,
    };

    static const int vertexSize = sizeof(vertices) / sizeof(Vertex);
    static const int indexSize = sizeof(indices) / sizeof(WORD);
}

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager)
	
public:

    void MakeResources();


    template<typename T>
    enable_if_t<is_base_of_v<Object,T>,bool> AddResource(string name,shared_ptr<T> resource);
    
    void RemoveResource(string name);

    template<typename T>
    shared_ptr<T> GetResource(string name);


private:
    unordered_map<string,shared_ptr<Object>> _resources;

};


template<typename T>
inline enable_if_t<is_base_of_v<Object, T>,bool> ResourceManager::AddResource(string name, shared_ptr<T> resource)
{
    if (_resources.find(name) == _resources.end())
    {
        _resources[name] = resource;
        return true;
    }
    return false;
}

template<typename T>
inline shared_ptr<T> ResourceManager::GetResource(string name)
{
    if (_resources.find(name) == _resources.end())
    {
        return nullptr;
    }
    return static_pointer_cast<T>(_resources[name]);
}

