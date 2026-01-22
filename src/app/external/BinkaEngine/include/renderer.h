#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "texturemgr.h"
#include "model.h"

#define INTERFACEVER_RENDERER "RendererV1"

enum RendererType
{
    RENDERER_NONE,
    RENDERER_SDL,
    RENDERER_OPENGL,
    RENDERER_OPENGLES,
    RENDERER_DIRECTX,
	RENDERER_VULKAN,
    NUM_RENDERERS
};

//Global functions
Uint32 GetRendererWindowFlags();
void SetRendererType();
bool CreateRenderer();
void DestroyRenderer();


class IRenderer
{
public:
    virtual ~IRenderer() {};

	virtual RendererType GetRendererType() const = 0;
	virtual const char* GetRendererName() const = 0;

    // Lifecycle
    virtual bool Init_Renderer() = 0;
    virtual void Setup_Renderer() = 0;
    virtual void Shutdown_Renderer() = 0;

    // Frame
    virtual void RenderClear() = 0;
    virtual void RenderPresent() = 0;

    // State
    virtual void SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;
    virtual void SetRenderDrawBlendMode(SDL_BlendMode blendMode) = 0;
    virtual void SetClipRect(SDL_Rect* rect) = 0;

    // Camera / View
    virtual void UpdateViewportScale() = 0;
    virtual void Setup2DView() = 0;
    virtual void SetupPerspective(float fovX, float fovY, float nearPlane, float farPlane) = 0;
    virtual void Begin3DSceneInRect(int x, int y, int width, int height, float fovX, float nearPlane, float farPlane) = 0;

    // Textures
    virtual Texture CreateTextureFromSurface(SDL_Surface* surface, const std::string& name, bool filtering = true) = 0;

    // 2D Drawing
    virtual void DrawTexturedRect(SDL_Rect rect, SDL_Color color, int textureID) = 0;
    virtual void DrawTextureNoScale(Texture tex, int x, int y) = 0;
    virtual void DrawLine(SDL_Point start, SDL_Point end, SDL_Color color, int thickness) = 0;
    virtual void DrawArrowLine(SDL_Point start, SDL_Point end, SDL_Color color, int thickness, int arrowLength, int arrowWidth) = 0;
    virtual void DrawPolygon(const std::vector<SDL_Point>& pts, int count, SDL_Color color) = 0;
    virtual void DrawPolygonOutline(const std::vector<SDL_Point>& pts, int count, int thickness, SDL_Color color) = 0;
    virtual void DrawFilledCircle(SDL_Point center, int radius, SDL_Color color, int segments) = 0;
    virtual void DrawCircle(SDL_Point center, int radius, SDL_Color color, int thickness, int segments) = 0;
    virtual void DrawRect(SDL_Rect rect, SDL_Color color) = 0;
    virtual void DrawFilledRect(SDL_Rect rect, SDL_Color color) = 0;

    // Text
    virtual void DrawFontAtlas(int fontID) = 0;
    virtual void FontDrawText(int fontID, const char* text, int x, int y) = 0;
    virtual void UpdateTextTexture(Texture& tex, int fontID, const char* text) = 0;

    // Saving
    virtual void SaveTextureAsBMP(SDL_Texture* tex, const char* path) = 0;

    // 3D
    virtual void DrawModel(ModelBuffer& buffer, const Vec3& position, const Vec3& rotation) = 0;


public:

};

extern int rendererID;
extern RendererType rendererType;
extern IRenderer* renderer_main;
extern bool renderer_initialized;
extern float scale_main;

#endif