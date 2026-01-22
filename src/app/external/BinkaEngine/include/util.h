#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <string>
#include <sstream>


struct KeyValue 
{
    std::string key;
    std::string value;
};

//Filename handling
std::string GetFilenameFromPathNoExt(const std::string& path);
std::string GetFilenameFromPath(const std::string& path);
std::string GetFileExt(const std::string& path);
std::string GetPathNoFilename(const std::string& path);
std::string RemoveDuplicateSlashes(const std::string& path);
std::string NormalizePath(const std::string& path);
std::string GetBasePath();
std::string GetFullPath(const std::string& relativePath);

//KeyValue
KeyValue ParseKeyValue(const std::string& line, bool* valueInQuotes = NULL);
int      ParseInt(const std::string& value);
float    ParseFloat(const std::string& value);

//Color
bool colorcmp(const SDL_Color& color1, const SDL_Color& color2);

//Math
bool     pointcmp(const SDL_Point& p1, const SDL_Point& p2);
SDL_Rect CenterRectInRect(const SDL_Rect& outer, const SDL_Rect& inner);
int      clamp(int value, int min, int max);
float    fclamp(float value, float min, float max);
int      getRandomInt(int min, int max);

SDL_FPoint GetViewportScaleRange(int width, int height);

//Strings
std::string getSubstringInRange(const std::string& str, size_t firstIndex, size_t lastIndex);
std::string unicodeToAscii(const std::string& input);
std::string findAndReplace(const std::string& input, const std::string& findStr, const std::string& replaceStr);


//Performance
void calculateFPS();
int  GetFPS();


#endif // UTIL_H
