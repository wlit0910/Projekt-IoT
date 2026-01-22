#ifndef BE_MATH_H
#define BE_MATH_H

#include <cmath>
#include <cstring>

#define M_PI_D 3.14159265358979323846
#define M_PI_F ((float)(M_PI_D))

#define DEG2RAD(x) ((x) * (M_PI_F / 180.0f))
#define RAD2DEG(x) ((x) * (180.0f / M_PI_F))

struct Vec3
{
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
};

struct Mat3
{
    float m[9]; // row-major layout

    void Identity();
    Mat3 Transpose() const;
};

struct Mat4
{
    float m[16]; // column-major (OpenGL format)

    void Identity();
    void Translate(const Vec3& v);
    void Rotate(const Vec3& eulerDeg);
    Mat4 operator*(const Mat4& rhs) const;
    void Multiply(const Mat4& rhs);
};

float Length(const Vec3& v);
Vec3 Normalize(const Vec3& v);
float Dot(const Vec3& a, const Vec3& b);
Vec3 Cross(const Vec3& a, const Vec3& b);

float ScaleFovX(float fovX, float width, float height);
float ScaleFovY(float fovX, float width, float height);

float NormalizeAngle360(float angle);
float NormalizeAngle180(float angle);
float NormalizeAngleSigned360(float angle);
Vec3 NormalizeAngles360(const Vec3& angles);
Vec3 NormalizeAnglesSigned360(const Vec3& angles);

#endif // BE_MATH_H
