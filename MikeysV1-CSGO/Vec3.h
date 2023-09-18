#include "pch.h"

#define PITCH 0 //x
#define YAW 1  //y
#define ROLL 2 //z
const float PITCH_MAX_DOWN = 90;
const float PITCH_MAX_UP = -90;
const float YAW_MAX_LEFT = 180;
const float YAW_MAX_RIGHT = -180;
static double pi = 3.14159265358;

class Vec3
{
public:

    union
    {
        struct { float x, y, z; };
        float v[3];
    };

    Vec3() { x = y = z = 0; }
    Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
    Vec3 operator + (const Vec3& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vec3 operator - (const Vec3& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vec3 operator * (const float& rhs) const { return Vec3(x * rhs, y * rhs, z * rhs); }
    Vec3 operator / (const float& rhs) const { return Vec3(x / rhs, y / rhs, z / rhs); }
    Vec3& operator + (Vec3 arg)
    { 
        x += arg.x;
        y += arg.y;
        z += arg.z;
        return *this;
    }
    Vec3& operator += (const Vec3& rhs) { return *this = *this + rhs; }
    Vec3& operator -= (const Vec3& rhs) { return *this = *this - rhs; }
    Vec3& operator *= (const float& rhs) { return *this = *this * rhs; }
    Vec3& operator /= (const float& rhs) { return *this = *this / rhs; }
    void RecoilNormalize()
    {
        while (y < -180)
            y += 360;

        while (y > 180)
            y -= 360;

        if (x > 89)
            x = 89;

        if (x < -89)
            x = -89;
    }
    float dot() const { return x * x + y * y + z * z; }
    float Length() const { return sqrtf(dot()); }
    Vec3 Normalize() const { return *this * (1 / Length()); }
    float Distance(const Vec3& rhs) const { return (*this - rhs).Length(); }
};

static float RadToDeg(float radian)
{
    return radian * (180 / pi);
}

static float DegToRad(float degree)
{
    return degree * (pi / 180);
}

static Vec3 RadToDeg(Vec3& radians)
{
    static Vec3 degrees;
    degrees.x = radians.x * (180 / pi);
    degrees.y = radians.y * (180 / pi);
    degrees.z = radians.z * (180 / pi);
    return degrees;
}

static Vec3 DegToRad(Vec3& degrees)
{
    static Vec3 radians;
    radians.x = degrees.x * (pi / 180);
    radians.y = degrees.y * (pi / 180);
    radians.z = degrees.z * (pi / 180);
    return radians;
}

static Vec3 CalcAngle(Vec3 src, Vec3 dst)
{
    static Vec3 angles;

    Vec3 delta = dst - src;
    float hyp = delta.Length();

    angles.v[PITCH] = RadToDeg(asinf(delta.v[ROLL] / hyp));
    angles.v[YAW] = RadToDeg(atan2f(delta.v[YAW], delta.v[PITCH]));

    if (PITCH_MAX_UP < 0)
    {
        angles.v[PITCH] = -angles.v[PITCH];
    }

    if (YAW_MAX_RIGHT > 0)
    {
        angles.v[YAW] = -angles.v[YAW];
    }

    if (YAW_MAX_RIGHT > 0)
    {
        angles.v[YAW] = angles.v[YAW] + 180.0f;
    }

    angles.v[ROLL] = 0.0f;

    return angles;
}

static Vec3 NormalizeDiff(Vec3 src, Vec3 dst)
{
    Vec3 diff = src - dst;

    if (abs(diff.v[1]) > YAW_MAX_LEFT)
        diff.v[1] = 360 - abs(diff.y);
    else
        diff.v[1] = abs(diff.y);
        
    if (abs(diff.v[0]) > PITCH_MAX_DOWN)
        diff.v[0] = 180 - abs(diff.x);
    else
        diff.v[0] = abs(diff.x);

    return diff;
}