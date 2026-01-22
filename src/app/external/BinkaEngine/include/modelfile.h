#ifndef MODELFILE_H
#define MODELFILE_H


enum ModelType
{
    MODEL_TYPE_STATIC = 0,
    MODEL_TYPE_DYNAMIC = 1
};


enum ModelFlags
{
    MODEL_FLAG_NONE = 0,
    MODEL_FLAG_HAS_TRANSPARENCY = 1
};

struct Vertex
{
    float pos[3];
    float norm[3];
    float uv[2];
};

// Main file header for the model
struct ModelFileHeader
{
    int magic;                // Magic number (e.g. 'BEMM')
    int version;              // Format version
    int flags;                // Reserved bitmask

    float min[3];             // AABB min corner
    float max[3];             // AABB max corner

    int modelType;            // Static (0) or dynamic (1)

    int numSubmodels;         // Number of submodels
    int submodelHeaderOffset; // Offset to submodel header array (from file start)

    int numMaterials;         // Total material strings
    int materialStringOffset; // Offset to string buffer
    int materialTextBufferSize; // Size of text buffer (in bytes)

    int reserved[4];          // Reserved for future use (e.g. skeleton/animation)
};

// Submodel metadata
struct SubmodelHeader
{
    int index;                 // Submodel index
    char name[32];             // Name (e.g. "body", "head")

    int vertexCount;
    int vertexBufferOffset;

    int indexCount;
    int indexBufferOffset;

    int textureParts;          // Number of texture regions
    int textureRangesOffset;   // Offset to TextureRange array
};

// Texture region for batching texture-dedicated draw calls
struct TextureRange
{
    int textureID;            // Index into material table
    int startIndex;           // First index in IBO
    int indexCount;           // Number of indices to draw
};

#endif // MODELFILE_H
