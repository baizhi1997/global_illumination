#include <math.h>
#include "vector.h"

Vector::Vector() : x(0), y(0), z(0) {}

Vector::Vector(float a_x, float a_y, float a_z)
: x(a_x), y(a_y), z(a_z) {}


const Vector& Vector::operator*=(float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

Vector Vector::operator*(float scale) const 
{
    return Vector(*this) *= scale;
}

const Vector& Vector::operator/=(float scale) 
{
    return operator*=(1.0f / scale);
}

Vector Vector::operator/(float scale) const 
{
    return Vector(*this) *= 1.0f / scale;
}

const Vector& Vector::operator+=(const Vector rhs) 
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector Vector::operator+(const Vector rhs) const 
{
    return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

const Vector& Vector::operator-=(const Vector rhs) 
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector Vector::operator-(const Vector rhs) const 
{
    return Vector(x -rhs.x, y - rhs.y, z - rhs.z);
}

const bool Vector::operator<(const Vector rhs) 
{
    float l1 = length();
    float l2 = rhs.length();
    return (l1 < l2);
}

void Vector::scale(float scale) 
{
    operator*=(scale);
}

Vector Vector::cross(const Vector rhs) const 
{
    float tx = y * rhs.z - z * rhs.y;
    float ty = z * rhs.x - x * rhs.z;
    float tz = x * rhs.y - y * rhs.x;
    return Vector(tx, ty, tz);
}

float Vector::dot(const Vector rhs) const 
{
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

float Vector::dist(const Vector rhs) const 
{
    Vector vdif = (*this)-rhs;
    return vdif.length(); 
}

float Vector::length() const 
{
    return sqrt(dot(*this));
}

void Vector::normalise() 
{
    scale(1.0f / length());
}