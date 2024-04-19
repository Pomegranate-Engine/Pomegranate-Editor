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
    struct Mat;


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
    
    struct Mat
    {
        std::vector<std::vector<float>> data;
        int rows;
        int cols;

        Mat();
        Mat(int rows, int cols);
        Mat(int rows, int cols, float** data);
        Mat(const Mat& other);
        ~Mat();

        Mat operator+(const Mat& other) const;
        Mat operator-(const Mat& other) const;
        Mat operator*(const Mat& other) const;
        Mat operator*(const float& other) const;
        Mat operator/(const Mat& other) const;
        Mat operator/(const float& other) const;
        Mat operator+=(const Mat& other);
        Mat operator-=(const Mat& other);
        Mat operator*=(const Mat& other);
        Mat operator*=(const float& other);
        Mat operator/=(const Mat& other);
        Mat operator/=(const float& other);
        bool operator==(const Mat& other) const;
        bool operator!=(const Mat& other) const;
        Vec2 operator*(const Vec2& other) const;
        Vec3 operator*(const Vec3& other) const;
        Vec4 operator*(const Vec4& other) const;
        Vec2i operator*(const Vec2i& other) const;
        Vec3i operator*(const Vec3i& other) const;
        Vec4i operator*(const Vec4i& other) const;

        Mat transpose() const;
        Mat inverse() const;
        Mat adjugate() const;
        float determinant() const;
        float cofactor(int row, int col) const;

        static Mat identity(int size);
        static Mat zero(int rows, int cols);
        static Mat perspective(float fov, float aspect, float near, float far);
        static Mat orthographic(float left, float right, float bottom, float top, float near, float far);
        static Mat look_at(const Vec3& eye, const Vec3& center, const Vec3& up);
        static Mat translate(const Vec3& translation);
        static Mat rotate(float angle, const Vec3& axis);
        static Mat scale(const Vec3& scale);
    };
}

#endif //POMEGRANATE_ENGINE_VEC_MATH_H
