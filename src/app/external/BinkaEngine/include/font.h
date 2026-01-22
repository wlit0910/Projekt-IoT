#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>
#include "texturemgr.h"

struct FontAtlas
{
    std::string font_name;
    std::string file_path;
    int         font_size;
    SDL_Color   font_color;
    SDL_Color   bg_color;
    bool        is_monospaced;
    int         max_glyph_width;
    int         max_glyph_height;
    Texture     texture;
    SDL_Rect glyphs[128];
};

#define INTERFACEVER_FONTSYSTEM "FontSystemV1"

class IFontSystem
{
public:
    virtual ~IFontSystem() {}

    virtual int  CreateFontAtlas(const std::string& font_name,
        const std::string& file_path,
        int font_size,
        SDL_Color font_color,
        SDL_Color bg_color) = 0;

    virtual int  GetFontIDBySpecs(const std::string& font_name,
        int font_size,
        SDL_Color font_color,
        SDL_Color bg_color) = 0;

    virtual FontAtlas* GetFontByID(int id) = 0;

    virtual SDL_Point GetTextSize(const char* text, const FontAtlas* atlas) = 0;
    virtual int       CountCharactersForWidth(const char* text,
        const FontAtlas* atlas,
        int maxWidth) = 0;
};

extern IFontSystem* g_fontsystem;


#endif