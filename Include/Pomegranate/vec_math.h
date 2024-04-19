#ifndef POMEGRANATE_ENGINE_VEC_MATH_H
#define POMEGRANATE_ENGINE_VEC_MATH_H
#include<cstdio>
#include <iostream>
#include <cmath>
namespace Pomegranate
{
    struct Vec2;
    struct Vec2i;
    struct Vec3;
    struct Vec3i;
    struct Vec4;
    struct Vec4i;
    struct Mat2x2;
    struct Mat3x3;
    struct Mat4x4;


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

        Vec2 operator*(const Mat2x2& other) const;
        Vec2 operator*=(const Mat2x2& other);
        Vec2 operator*(const Mat3x3& other) const;
        Vec2 operator*=(const Mat3x3& other);
        Vec2 operator*(const Mat4x4& other) const;
        Vec2 operator*=(const Mat4x4& other);

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

        Vec2i operator*(const Mat2x2& other) const;
        Vec2i operator*=(const Mat2x2& other);
        Vec2i operator*(const Mat3x3& other) const;
        Vec2i operator*=(const Mat3x3& other);
        Vec2i operator*(const Mat4x4& other) const;
        Vec2i operator*=(const Mat4x4& other);

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

        Vec3 operator*(const Mat3x3& other) const;
        Vec3 operator*=(const Mat3x3& other);
        Vec3 operator*(const Mat4x4& other) const;
        Vec3 operator*=(const Mat4x4& other);

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

    struct Vec3i
    {
        int x;
        int y;
        int z;

        Vec3i();
        Vec3i(int x, int y, int z);
        Vec3i operator+(const Vec3i& other) const;
        Vec3i operator-(const Vec3i& other) const;
        Vec3i operator*(const Vec3i& other) const;
        Vec3i operator*(const float& other) const;
        Vec3i operator/(const Vec3i& other) const;
        Vec3i operator/(const float& other) const;
        Vec3i operator+=(const Vec3i& other);
        Vec3i operator-=(const Vec3i& other);
        Vec3i operator*=(const Vec3i& other);
        Vec3i operator*=(const float& other);
        Vec3i operator/=(const Vec3i& other);
        Vec3i operator/=(const float& other);

        Vec3i operator*(const Mat3x3& other) const;
        Vec3i operator*=(const Mat3x3& other);
        Vec3i operator*(const Mat4x4& other) const;
        Vec3i operator*=(const Mat4x4& other);

        bool operator==(const Vec3i& other) const;
        bool operator!=(const Vec3i& other) const;
        float dot(const Vec3i& other) const;
        Vec3i cross(const Vec3i& other) const;
        float length() const;
        Vec3i normalized() const;
        Vec3i rotate(float angle, const Vec3i& axis) const;
        Vec3i lerp(const Vec3i& other, float t) const;
        Vec3i slerp(const Vec3i& other, float t) const;
        Vec3i direction_to(const Vec3i& other) const;
        float distance_to(const Vec3i& other) const;
        float angle_to(const Vec3i& other) const;
    };

    struct Vec4
    {
        float x;
        float y;
        float z;
        float w;

        Vec4();
        Vec4(float x, float y, float z, float w);
        Vec4 operator+(const Vec4& other) const;
        Vec4 operator-(const Vec4& other) const;
        Vec4 operator*(const Vec4& other) const;
        Vec4 operator*(const float& other) const;
        Vec4 operator/(const Vec4& other) const;
        Vec4 operator/(const float& other) const;
        Vec4 operator+=(const Vec4& other);
        Vec4 operator-=(const Vec4& other);
        Vec4 operator*=(const Vec4& other);
        Vec4 operator*=(const float& other);
        Vec4 operator/=(const Vec4& other);
        Vec4 operator/=(const float& other);
        bool operator==(const Vec4& other) const;
        bool operator!=(const Vec4& other) const;

        Vec4 operator*(const Mat4x4& other) const;
        Vec4 operator*=(const Mat4x4& other);

        float dot(const Vec4& other) const;
        Vec4 cross(const Vec4& other) const;
        float length() const;
        Vec4 normalized() const;
        Vec4 rotate(float angle, const Vec4& axis) const;
        Vec4 lerp(const Vec4& other, float t) const;
        Vec4 slerp(const Vec4& other, float t) const;
        Vec4 direction_to(const Vec4& other) const;
        float distance_to(const Vec4& other) const;
        float angle_to(const Vec4& other) const;
    };

    struct Vec4i
    {
        int x;
        int y;
        int z;
        int w;

        Vec4i();
        Vec4i(int x, int y, int z, int w);
        Vec4i operator+(const Vec4i& other) const;
        Vec4i operator-(const Vec4i& other) const;
        Vec4i operator*(const Vec4i& other) const;
        Vec4i operator*(const float& other) const;
        Vec4i operator/(const Vec4i& other) const;
        Vec4i operator/(const float& other) const;
        Vec4i operator+=(const Vec4i& other);
        Vec4i operator-=(const Vec4i& other);
        Vec4i operator*=(const Vec4i& other);
        Vec4i operator*=(const float& other);
        Vec4i operator/=(const Vec4i& other);
        Vec4i operator/=(const float& other);
        bool operator==(const Vec4i& other) const;
        bool operator!=(const Vec4i& other) const;
        float dot(const Vec4i& other) const;
        Vec4i cross(const Vec4i& other) const;
        float length() const;
        Vec4i normalized() const;
        Vec4i rotate(float angle, const Vec4i& axis) const;
        Vec4i lerp(const Vec4i& other, float t) const;
        Vec4i slerp(const Vec4i& other, float t) const;
        Vec4i direction_to(const Vec4i& other) const;
        float distance_to(const Vec4i& other) const;
        float angle_to(const Vec4i& other) const;
    };
    
    struct Mat2x2
    {
        float m00, m01, m10, m11;

        Mat2x2();
        Mat2x2(float m00, float m01, float m10, float m11);
        Mat2x2 operator+(const Mat2x2& other) const;
        Mat2x2 operator-(const Mat2x2& other) const;
        Mat2x2 operator*(const Mat2x2& other) const;
        Mat2x2 operator*(const float& other) const;
        Mat2x2 operator/(const Mat2x2& other) const;
        Mat2x2 operator/(const float& other) const;
        Mat2x2 operator+=(const Mat2x2& other);
        Mat2x2 operator-=(const Mat2x2& other);
        Mat2x2 operator*=(const Mat2x2& other);
        Mat2x2 operator*=(const float& other);
        Mat2x2 operator/=(const Mat2x2& other);
        Mat2x2 operator/=(const float& other);
        bool operator==(const Mat2x2& other) const;
        bool operator!=(const Mat2x2& other) const;
        float determinant() const;
        Mat2x2 inverse() const;
        Mat2x2 transpose() const;
    };

    struct Mat3x3
    {
        float m00, m01, m02, m10, m11, m12, m20, m21, m22;

        Mat3x3();
        Mat3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
        Mat3x3 operator+(const Mat3x3& other) const;
        Mat3x3 operator-(const Mat3x3& other) const;
        Mat3x3 operator*(const Mat3x3& other) const;
        Mat3x3 operator*(const float& other) const;
        Mat3x3 operator/(const Mat3x3& other) const;
        Mat3x3 operator/(const float& other) const;
        Mat3x3 operator+=(const Mat3x3& other);
        Mat3x3 operator-=(const Mat3x3& other);
        Mat3x3 operator*=(const Mat3x3& other);
        Mat3x3 operator*=(const float& other);
        Mat3x3 operator/=(const Mat3x3& other);
        Mat3x3 operator/=(const float& other);
        bool operator==(const Mat3x3& other) const;
        bool operator!=(const Mat3x3& other) const;
        float determinant() const;
        Mat3x3 inverse() const;
        Mat3x3 transpose() const;
    };

    struct Mat4x4
    {
        float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;

        Mat4x4();
        Mat4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
        Mat4x4 operator+(const Mat4x4& other) const;
        Mat4x4 operator-(const Mat4x4& other) const;
        Mat4x4 operator*(const Mat4x4& other) const;
        Mat4x4 operator*(const float& other) const;
        Mat4x4 operator/(const Mat4x4& other) const;
        Mat4x4 operator/(const float& other) const;
        Mat4x4 operator+=(const Mat4x4& other);
        Mat4x4 operator-=(const Mat4x4& other);
        Mat4x4 operator*=(const Mat4x4& other);
        Mat4x4 operator*=(const float& other);
        Mat4x4 operator/=(const Mat4x4& other);
        Mat4x4 operator/=(const float& other);
        bool operator==(const Mat4x4& other) const;
        bool operator!=(const Mat4x4& other) const;
        float determinant() const;
        Mat4x4 inverse() const;
        Mat4x4 transpose() const;
    };
}

#endif //POMEGRANATE_ENGINE_VEC_MATH_H
