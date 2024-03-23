#ifndef POMEGRANATE_ENGINE_VEC_MATH_H
#define POMEGRANATE_ENGINE_VEC_MATH_H
#include<cstdio>
#include <iostream>
#include <cmath>
namespace Pomegranate
{
    struct Vec2
    {
        float x;
        float y;

        Vec2();
        Vec2(float x, float y);
        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        Vec2 operator*(const Vec2& other) const;
        Vec2 operator*(const float& other) const;
        Vec2 operator/(const Vec2& other) const;
        Vec2 operator/(const float& other) const;
        Vec2 operator+=(const Vec2& other);
        Vec2 operator-=(const Vec2& other);
        Vec2 operator*=(const Vec2& other);
        Vec2 operator*=(const float& other);
        Vec2 operator/=(const Vec2& other);
        Vec2 operator/=(const float& other);
        bool operator==(const Vec2& other) const;
        bool operator!=(const Vec2& other) const;
        float dot(const Vec2& other) const;
        float cross(const Vec2& other) const;
        float length() const;
        Vec2 normalized() const;
        Vec2 rotate(float angle) const;
        Vec2 lerp(const Vec2& other, float t) const;
        Vec2 slerp(const Vec2& other, float t) const;
        Vec2 direction_to(const Vec2& other) const;
        float distance_to(const Vec2& other) const;
        float angle_to(const Vec2& other) const;
    };

    struct Vec2i
    {
        int x;
        int y;

        Vec2i();
        Vec2i(int x, int y);
        Vec2i operator+(const Vec2i& other) const;
        Vec2i operator-(const Vec2i& other) const;
        Vec2i operator*(const Vec2i& other) const;
        Vec2i operator*(const float& other) const;
        Vec2i operator/(const Vec2i& other) const;
        Vec2i operator/(const float& other) const;
        Vec2i operator+=(const Vec2i& other);
        Vec2i operator-=(const Vec2i& other);
        Vec2i operator*=(const Vec2i& other);
        Vec2i operator*=(const float& other);
        Vec2i operator/=(const Vec2i& other);
        Vec2i operator/=(const float& other);
        bool operator==(const Vec2i& other) const;
        bool operator!=(const Vec2i& other) const;
        float dot(const Vec2i& other) const;
        float cross(const Vec2i& other) const;
        float length() const;
        Vec2i normalized() const;
        Vec2i rotate(float angle) const;
        Vec2i lerp(const Vec2i& other, float t) const;
        Vec2i slerp(const Vec2i& other, float t) const;
        Vec2i direction_to(const Vec2i& other) const;
        float distance_to(const Vec2i& other) const;
        float angle_to(const Vec2i& other) const;
    };

    struct Vec3
    {
        float x;
        float y;
        float z;

        Vec3();
        Vec3(float x, float y, float z);
        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;
        Vec3 operator*(const Vec3& other) const;
        Vec3 operator*(const float& other) const;
        Vec3 operator/(const Vec3& other) const;
        Vec3 operator/(const float& other) const;
        Vec3 operator+=(const Vec3& other);
        Vec3 operator-=(const Vec3& other);
        Vec3 operator*=(const Vec3& other);
        Vec3 operator*=(const float& other);
        Vec3 operator/=(const Vec3& other);
        Vec3 operator/=(const float& other);
        bool operator==(const Vec3& other) const;
        bool operator!=(const Vec3& other) const;
        float dot(const Vec3& other) const;
        Vec3 cross(const Vec3& other) const;
        float length() const;
        Vec3 normalized() const;
        Vec3 rotate(float angle, const Vec3& axis) const;
        Vec3 lerp(const Vec3& other, float t) const;
        Vec3 slerp(const Vec3& other, float t) const;
        Vec3 direction_to(const Vec3& other) const;
        float distance_to(const Vec3& other) const;
        float angle_to(const Vec3& other) const;
    };
}

#endif //POMEGRANATE_ENGINE_VEC_MATH_H
