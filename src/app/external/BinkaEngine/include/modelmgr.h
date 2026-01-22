#ifndef MODELMGR_H
#define MODELMGR_H

#include <string>
#include <vector>
#include "modelfile.h"
#include "texturemgr.h"



struct ModelBuffer
{
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<TextureRange> textureRanges;
    std::vector<std::string> textureNames;
    std::vector<int> textureIDs;

    Uint32 vbo = 0;
    Uint32 ibo = 0;
    bool gpuUploaded = false;


    int refCount = 0;
    Uint64 unloadTime = 0;
    bool no_unload = false;
};

#define INTERFACEVER_MODELMGR "ModelMgrV1"


class IModelMgr
{
public:
    virtual ~IModelMgr() {}

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;

    virtual int LoadModel(const std::string& path) = 0;
    virtual int LoadModelCritical(const std::string& path) = 0;

    virtual void RequestModel(const std::string& path, int& model_id) = 0;
    virtual void ReleaseModel(int id) = 0;

    virtual ModelBuffer* GetModelByID(int id) = 0;
    virtual void DestroyModelByID(int id) = 0;

    virtual void RemoveAllModels() = 0;
    virtual void Update() = 0;
};

extern IModelMgr* g_modelmgr;



bool Init_ModelMgr();
void Shutdown_ModelMgr();
void Update_ModelMgr();


#endif
