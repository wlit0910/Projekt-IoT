#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

#define INTERFACEVER_FILESYSTEM "FilesystemV1"


class IFileSystem
{
public:
    virtual ~IFileSystem() {}

    virtual std::string GetTexturePath(const std::string& relativePath) = 0;
    virtual std::string GetMaterialPath(const std::string& relativePath) = 0;
    virtual std::string GetModelPath(const std::string& relativePath) = 0;
    virtual std::string GetShaderPath(const std::string& relativePath) = 0;
};

extern const char* g_szGameFolder;
extern IFileSystem* g_filesystem;

#endif // FILESYSTEM_H