#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "be_math.h"
#include "modelmgr.h"

class Model
{
public:
    Model();
    Model(const std::string& path);
    virtual ~Model();

    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);
    void Draw();

    const Vec3& GetPosition() const;
    const Vec3& GetRotation() const;

private:
    int model_id;
    Vec3 position;
    Vec3 rotation;
};

#endif // MODEL_H
