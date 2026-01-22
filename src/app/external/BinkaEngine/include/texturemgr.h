#ifndef TEXTUREMGR_H
#define TEXTUREMGR_H

#include <SDL.h>
#include <string>
#include <vector>

struct Texture
{
    std::string name;
    Uint64 unloadTime;
    //union
    //{
        SDL_Texture* sdlTexture;
        Uint32 glTextureID;
    //};

    int width;
    int height;
    int refCount;
    bool noUnload;
};

#define INTERFACEVER_TEXTUREMGR "TextureMgrV1"

class ITextureMgr
{
public:
    virtual ~ITextureMgr() {}

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;

    virtual int LoadTexture(const std::string& file_path) = 0;
    virtual int LoadTextureCritical(const std::string& file_path) = 0;

    virtual void DestroyTextureByID(int id) = 0;
    virtual void DestroyTexture(Texture* texture) = 0;

    virtual void RemoveAllTextures() = 0;

    virtual int          GetTextureIDByName(const std::string& name) = 0;
    virtual SDL_Texture* GetSDLTextureByID(int id) = 0;
    virtual Uint32       GetGLTextureByID(int id) = 0;
    virtual Texture* GetTextureByID(int id) = 0;

    virtual void RequestTexture(const std::string& name, int& texture_id) = 0;
    virtual void ReleaseTexture(int id) = 0;

    virtual void Update() = 0;
};

extern ITextureMgr* g_texturemgr;


bool         Init_TexMgr();
void         Shutdown_TexMgr();
void         Update_TexMgr();

#endif