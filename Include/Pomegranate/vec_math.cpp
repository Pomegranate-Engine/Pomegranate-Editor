#include "vec_math.h"
namespace Pomegranate
{
    Vec2::Vec2()
    {
        this->x = 0.0;
        this->y = 0.0;
    }
    Vec2::Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    Vec2 Vec2::operator+(const Vec2& other) const
    {
        return {this->x + other.x, this->y + other.y};
    }
    Vec2 Vec2::operator-(const Vec2& other) const
    {
        return {this->x - other.x, this->y - other.y};
    }
    Vec2 Vec2::operator*(const Vec2& other) const
    {
        return {this->x * other.x, this->y * other.y};
    }
    Vec2 Vec2::operator*(const float& other) const
    {
        return {this->x * other, this->y * other};
    }
    Vec2 Vec2::operator/(const Vec2& other) const
    {
        return {this->x / other.x, this->y / other.y};
    }
    Vec2 Vec2::operator/(const float& other) const
    {
        return {this->x / other, this->y / other};
    }
    Vec2 Vec2::operator+=(const Vec2& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Vec2 Vec2::operator-=(const Vec2& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
    Vec2 Vec2::operator*=(const Vec2& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }
    Vec2 Vec2::operator*=(const float& other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }
    Vec2 Vec2::operator/=(const Vec2& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }
    Vec2 Vec2::operator/=(const float& other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }
    bool Vec2::operator==(const Vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
    bool Vec2::operator!=(const Vec2& other) const
    {
        return this->x != other.x || this->y != other.y;
    }
    Vec2 Vec2::operator*(const Mat2x2& other) const
    {
        return {this->x * other.m00 + this->y * other.m10, this->x * other.m01 + this->y * other.m11};
    }
    Vec2 Vec2::operator*=(const Mat2x2& other)
    {
        float x = this->x;
        this->x = x * other.m00 + this->y * other.m10;
        this->y = x * other.m01 + this->y * other.m11;
        return *this;
    }
    Vec2 Vec2::operator*(const Mat3x3& other) const
    {
        return {this->x * other.m00 + this->y * other.m10 + other.m20, this->x * other.m01 + this->y * other.m11 + other.m21};
    }
    Vec2 Vec2::operator*=(const Mat3x3& other)
    {
        float x = this->x;
        this->x = x * other.m00 + this->y * other.m10 + other.m20;
        this->y = x * other.m01 + this->y * other.m11 + other.m21;
        return *this;
    }
    Vec2 Vec2::operator*(const Mat4x4& other) const
    {
        return {this->x * other.m00 + this->y * other.m10 + other.m30, this->x * other.m01 + this->y * other.m11 + other.m31};
    }
    Vec2 Vec2::operator*=(const Mat4x4& other)
    {
        float x = this->x;
        this->x = x * other.m00 + this->y * other.m10 + other.m30;
        this->y = x * other.m01 + this->y * other.m11 + other.m31;
        return *this;
    }
    float Vec2::dot(const Vec2& other) const
    {
        return this->x * other.x + this->y * other.y;
    }
    float Vec2::cross(const Vec2& other) const
    {
        return this->x * other.y - this->y * other.x;
    }
    float Vec2::length() const
    {
        return sqrt(this->x * this->x + this->y * this->y);
    }
    Vec2 Vec2::normalized() const
    {
        float l = this->length();
        return {this->x / l, this->y / l};
    }
    Vec2 Vec2::rotate(float angle) const
    {
        return {this->x * cos(angle) - this->y * sin(angle), this->x * sin(angle) + this->y * cos(angle)};
    }
    Vec2 Vec2::lerp(const Vec2& other, float t) const
    {
        return {this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t};
    }
    Vec2 Vec2::slerp(const Vec2& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t) * (1.0f - t)) + (other * t);
    }
    Vec2 Vec2::direction_to(const Vec2& other) const
    {
        return (other - *this).normalized();
    }
    float Vec2::distance_to(const Vec2& other) const
    {
        return (other - *this).length();
    }
    float Vec2::angle_to(const Vec2& other) const
    {
        return atan2(other.y - this->y, other.x - this->x);
    }



    Vec2i::Vec2i()
    {
        this->x = 0.0;
        this->y = 0.0;
    }
    Vec2i::Vec2i(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    Vec2i Vec2i::operator+(const Vec2i& other) const
    {
        return {this->x + other.x, this->y + other.y};
    }
    Vec2i Vec2i::operator-(const Vec2i& other) const
    {
        return {this->x - other.x, this->y - other.y};
    }
    Vec2i Vec2i::operator*(const Vec2i& other) const
    {
        return {this->x * other.x, this->y * other.y};
    }
    Vec2i Vec2i::operator*(const float& other) const
    {
        return {(int)((float)this->x * other), (int)((float)this->y * other)};
    }
    Vec2i Vec2i::operator/(const Vec2i& other) const
    {
        return {this->x / other.x, this->y / other.y};
    }
    Vec2i Vec2i::operator/(const float& other) const
    {
        return {(int)((float)this->x / other), (int)((float)this->y / other)};
    }
    Vec2i Vec2i::operator+=(const Vec2i& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Vec2i Vec2i::operator-=(const Vec2i& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
    Vec2i Vec2i::operator*=(const Vec2i& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }
    Vec2i Vec2i::operator*=(const float& other)
    {
        this->x *= (int)other;
        this->y *= (int)other;
        return *this;
    }
    Vec2i Vec2i::operator/=(const Vec2i& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }
    Vec2i Vec2i::operator/=(const float& other)
    {
        this->x /= (int)other;
        this->y /= (int)other;
        return *this;
    }
    bool Vec2i::operator==(const Vec2i& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
    bool Vec2i::operator!=(const Vec2i& other) const
    {
        return this->x != other.x || this->y != other.y;
    }
    Vec2i Vec2i::operator*(const Mat2x2& other) const
    {
        return {(int)((float)this->x * other.m00 + (float)this->y * other.m10), (int)((float)this->x * other.m01 + (float)this->y * other.m11)};
    }
    Vec2i Vec2i::operator*=(const Mat2x2& other)
    {
        int x = this->x;
        this->x = (int)((float)x * other.m00 + (float)this->y * other.m10);
        this->y = (int)((float)x * other.m01 + (float)this->y * other.m11);
        return *this;
    }
    Vec2i Vec2i::operator*(const Mat3x3& other) const
    {
        return {(int)((float)this->x * other.m00 + (float)this->y * other.m10 + other.m20), (int)((float)this->x * other.m01 + (float)this->y * other.m11 + other.m21)};
    }
    Vec2i Vec2i::operator*=(const Mat3x3& other)
    {
        int x = this->x;
        this->x = (int)((float)x * other.m00 + (float)this->y * other.m10 + other.m20);
        this->y = (int)((float)x * other.m01 + (float)this->y * other.m11 + other.m21);
        return *this;
    }
    Vec2i Vec2i::operator*(const Mat4x4& other) const
    {
        return {(int)((float)this->x * other.m00 + (float)this->y * other.m10 + other.m30), (int)((float)this->x * other.m01 + (float)this->y * other.m11 + other.m31)};
    }
    Vec2i Vec2i::operator*=(const Mat4x4& other)
    {
        int x = this->x;
        this->x = (int)((float)x * other.m00 + (float)this->y * other.m10 + other.m30);
        this->y = (int)((float)x * other.m01 + (float)this->y * other.m11 + other.m31);
        return *this;
    }
    float Vec2i::dot(const Vec2i& other) const
    {
        return (float)this->x * (float)other.x + (float)this->y * (float)other.y;
    }
    float Vec2i::cross(const Vec2i& other) const
    {
        return (float)this->x * (float)other.y - (float)this->y * (float)other.x;
    }
    float Vec2i::length() const
    {
        return sqrt((float)this->x * (float)this->x + (float)this->y * (float)this->y);
    }
    Vec2i Vec2i::normalized() const
    {
        float l = this->length();
        return {(int)((float)this->x / l), (int)((float)this->y / l)};
    }
    Vec2i Vec2i::rotate(float angle) const
    {
        return {(int)((float)this->x * cos(angle) - (float)this->y * sin(angle)), (int)((float)this->x * sin(angle) + (float)this->y * cos(angle))};
    }
    Vec2i Vec2i::lerp(const Vec2i& other, float t) const
    {
        return {(int)((float)this->x + ((float)other.x - (float)this->x) * t), (int)((float)this->y + ((float)other.y - (float)this->y) * t)};
    }
    Vec2i Vec2i::slerp(const Vec2i& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t) * (1.0f - t)) + (other * t);
    }
    Vec2i Vec2i::direction_to(const Vec2i& other) const
    {
        return (other - *this).normalized();
    }
    float Vec2i::distance_to(const Vec2i& other) const
    {
        return (other - *this).length();
    }
    float Vec2i::angle_to(const Vec2i& other) const
    {
        return atan2((float)other.y - (float)this->y, (float)other.x - (float)this->x);
    }


    Vec3::Vec3()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    Vec3::Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3 Vec3::operator+(const Vec3& other) const
    {
        return {this->x + other.x, this->y + other.y, this->z + other.z};
    }
    Vec3 Vec3::operator-(const Vec3& other) const
    {
        return {this->x - other.x, this->y - other.y, this->z - other.z};
    }
    Vec3 Vec3::operator*(const Vec3& other) const
    {
        return {this->x * other.x, this->y * other.y, this->z * other.z};
    }
    Vec3 Vec3::operator*(const float& other) const
    {
        return {this->x * other, this->y * other, this->z * other};
    }
    Vec3 Vec3::operator/(const Vec3& other) const
    {
        return {this->x / other.x, this->y / other.y, this->z / other.z};
    }
    Vec3 Vec3::operator/(const float& other) const
    {
        return {this->x / other, this->y / other, this->z / other};
    }
    Vec3 Vec3::operator+=(const Vec3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    Vec3 Vec3::operator-=(const Vec3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }
    Vec3 Vec3::operator*=(const Vec3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }
    Vec3 Vec3::operator*=(const float& other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }
    Vec3 Vec3::operator/=(const Vec3& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }
    Vec3 Vec3::operator/=(const float& other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        return *this;
    }
    bool Vec3::operator==(const Vec3& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }
    bool Vec3::operator!=(const Vec3& other) const
    {
        return this->x != other.x || this->y != other.y || this->z != other.z;
    }
    Vec3 Vec3::operator*(const Mat3x3& other) const
    {
        return {this->x * other.m00 + this->y * other.m10 + this->z * other.m20, this->x * other.m01 + this->y * other.m11 + this->z * other.m21, this->x * other.m02 + this->y * other.m12 + this->z * other.m22};
    }
    Vec3 Vec3::operator*=(const Mat3x3& other)
    {
        float x = this->x;
        float y = this->y;
        this->x = x * other.m00 + y * other.m10 + this->z * other.m20;
        this->y = x * other.m01 + y * other.m11 + this->z * other.m21;
        this->z = x * other.m02 + y * other.m12 + this->z * other.m22;
        return *this;
    }
    Vec3 Vec3::operator*(const Mat4x4& other) const
    {
        return {this->x * other.m00 + this->y * other.m10 + this->z * other.m20 + other.m30, this->x * other.m01 + this->y * other.m11 + this->z * other.m21 + other.m31, this->x * other.m02 + this->y * other.m12 + this->z * other.m22 + other.m32};
    }
    Vec3 Vec3::operator*=(const Mat4x4& other)
    {
        float x = this->x;
        float y = this->y;
        this->x = x * other.m00 + y * other.m10 + this->z * other.m20 + other.m30;
        this->y = x * other.m01 + y * other.m11 + this->z * other.m21 + other.m31;
        this->z = x * other.m02 + y * other.m12 + this->z * other.m22 + other.m32;
        return *this;
    }
    float Vec3::dot(const Vec3& other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }
    Vec3 Vec3::cross(const Vec3& other) const
    {
        return {this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x};
    }
    float Vec3::length() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }
    Vec3 Vec3::normalized() const
    {
        float l = this->length();
        return {this->x / l, this->y / l, this->z / l};
    }
    Vec3 Vec3::rotate(float angle, const Vec3& axis) const
    {
        float s = sin(angle);
        float c = cos(angle);
        float oc = 1.0f - c;
        return {this->x * (oc * axis.x * axis.x + c) + this->y * (oc * axis.x * axis.y - axis.z * s) + this->z * (oc * axis.x * axis.z + axis.y * s),
                this->x * (oc * axis.x * axis.y + axis.z * s) + this->y * (oc * axis.y * axis.y + c) + this->z * (oc * axis.y * axis.z - axis.x * s),
                this->x * (oc * axis.x * axis.z - axis.y * s) + this->y * (oc * axis.y * axis.z + axis.x * s) + this->z * (oc * axis.z * axis.z + c)};
    }
    Vec3 Vec3::lerp(const Vec3& other, float t) const
    {
        return {this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t, this->z + (other.z - this->z) * t};
    }
    Vec3 Vec3::slerp(const Vec3& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t, this->cross(other)) * (1.0f - t)) + (other * t);
    }
    Vec3 Vec3::direction_to(const Vec3& other) const
    {
        return (other - *this).normalized();
    }
    float Vec3::distance_to(const Vec3& other) const
    {
        return (other - *this).length();
    }
    float Vec3::angle_to(const Vec3& other) const
    {
        return atan2(other.y - this->y, other.x - this->x);
    }

    Vec3i::Vec3i()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    Vec3i::Vec3i(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3i Vec3i::operator+(const Vec3i& other) const
    {
        return {this->x + other.x, this->y + other.y, this->z + other.z};
    }
    Vec3i Vec3i::operator-(const Vec3i& other) const
    {
        return {this->x - other.x, this->y - other.y, this->z - other.z};
    }
    Vec3i Vec3i::operator*(const Vec3i& other) const
    {
        return {this->x * other.x, this->y * other.y, this->z * other.z};
    }
    Vec3i Vec3i::operator*(const float& other) const
    {
        return {(int)((float)this->x * other), (int)((float)this->y * other), (int)((float)this->z * other)};
    }
    Vec3i Vec3i::operator/(const Vec3i& other) const
    {
        return {this->x / other.x, this->y / other.y, this->z / other.z};
    }
    Vec3i Vec3i::operator/(const float& other) const
    {
        return {(int)((float)this->x / other), (int)((float)this->y / other), (int)((float)this->z / other)};
    }
    Vec3i Vec3i::operator+=(const Vec3i& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    Vec3i Vec3i::operator-=(const Vec3i& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }
    Vec3i Vec3i::operator*=(const Vec3i& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }
    Vec3i Vec3i::operator*=(const float& other)
    {
        this->x *= (int)other;
        this->y *= (int)other;
        this->z *= (int)other;
        return *this;
    }
    Vec3i Vec3i::operator/=(const Vec3i& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }
    Vec3i Vec3i::operator/=(const float& other)
    {
        this->x /= (int)other;
        this->y /= (int)other;
        this->z /= (int)other;
        return *this;
    }
    bool Vec3i::operator==(const Vec3i& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }
    bool Vec3i::operator!=(const Vec3i& other) const
    {
        return this->x != other.x || this->y != other.y || this->z != other.z;
    }
    Vec3i Vec3i::operator*(const Mat3x3& other) const
    {
        return {(int)((float)this->x * other.m00 + (float)this->y * other.m10 + other.m20), (int)((float)this->x * other.m01 + (float)this->y * other.m11 + other.m21), (int)((float)this->x * other.m02 + (float)this->y * other.m12 + other.m22)};
    }
    Vec3i Vec3i::operator*=(const Mat3x3& other)
    {
        int x = this->x;
        int y = this->y;
        this->x = (int)((float)x * other.m00 + (float)y * other.m10 + other.m20);
        this->y = (int)((float)x * other.m01 + (float)y * other.m11 + other.m21);
        this->z = (int)((float)x * other.m02 + (float)y * other.m12 + other.m22);
        return *this;
    }
    Vec3i Vec3i::operator*(const Mat4x4& other) const
    {
        return {(int)((float)this->x * other.m00 + (float)this->y * other.m10 + other.m30), (int)((float)this->x * other.m01 + (float)this->y * other.m11 + other.m31), (int)((float)this->x * other.m02 + (float)this->y * other.m12 + other.m32)};
    }
    Vec3i Vec3i::operator*=(const Mat4x4& other)
    {
        int x = this->x;
        int y = this->y;
        this->x = (int)((float)x * other.m00 + (float)y * other.m10 + other.m30);
        this->y = (int)((float)x * other.m01 + (float)y * other.m11 + other.m31);
        this->z = (int)((float)x * other.m02 + (float)y * other.m12 + other.m32);
        return *this;
    }
    float Vec3i::dot(const Vec3i& other) const
    {
        return (float)this->x * (float)other.x + (float)this->y * (float)other.y + (float)this->z * (float)other.z;
    }
    Vec3i Vec3i::cross(const Vec3i& other) const
    {
        return {(int)((float)this->y * (float)other.z - (float)this->z * (float)other.y), (int)((float)this->z * (float)other.x - (float)this->x * (float)other.z), (int)((float)this->x * (float)other.y - (float)this->y * (float)other.x)};
    }
    float Vec3i::length() const
    {
        return sqrt((float)this->x * (float)this->x + (float)this->y * (float)this->y + (float)this->z * (float)this->z);
    }
    Vec3i Vec3i::normalized() const
    {
        float l = this->length();
        return {(int)((float)this->x / l), (int)((float)this->y / l), (int)((float)this->z / l)};
    }
    Vec3i Vec3i::rotate(float angle, const Vec3i& axis) const
    {
        return {(int)((float)this->x * cos(angle) - (float)this->y * sin(angle)), (int)((float)this->x * sin(angle) + (float)this->y * cos(angle)), (int)((float)this->z)};
    }
    Vec3i Vec3i::lerp(const Vec3i& other, float t) const
    {
        return {(int)((float)this->x + ((float)other.x - (float)this->x) * t), (int)((float)this->y + ((float)other.y - (float)this->y) * t), (int)((float)this->z + ((float)other.z - (float)this->z) * t)};
    }
    Vec3i Vec3i::slerp(const Vec3i& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t, this->cross(other)) * (1.0f - t)) + (other * t);
    }
    Vec3i Vec3i::direction_to(const Vec3i& other) const
    {
        return (other - *this).normalized();
    }
    float Vec3i::distance_to(const Vec3i& other) const
    {
        return (other - *this).length();
    }
    float Vec3i::angle_to(const Vec3i& other) const
    {
        return atan2((float)other.y - (float)this->y, (float)other.x - (float)this->x);
    }

    Vec4::Vec4()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        this->w = 0.0;
    }
    Vec4::Vec4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vec4 Vec4::operator+(const Vec4& other) const
    {
        return {this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w};
    }
    Vec4 Vec4::operator-(const Vec4& other) const
    {
        return {this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w};
    }
    Vec4 Vec4::operator*(const Vec4& other) const
    {
        return {this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w};
    }
    Vec4 Vec4::operator*(const float& other) const
    {
        return {this->x * other, this->y * other, this->z * other, this->w * other};
    }
    Vec4 Vec4::operator/(const Vec4& other) const
    {
        return {this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w};
    }
    Vec4 Vec4::operator/(const float& other) const
    {
        return {this->x / other, this->y / other, this->z / other, this->w / other};
    }
    Vec4 Vec4::operator+=(const Vec4& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }
    Vec4 Vec4::operator-=(const Vec4& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }
    Vec4 Vec4::operator*=(const Vec4& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        this->w *= other.w;
        return *this;
    }
    Vec4 Vec4::operator*=(const float& other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        this->w *= other;
        return *this;
    }
    Vec4 Vec4::operator/=(const Vec4& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        this->w /= other.w;
        return *this;
    }
    Vec4 Vec4::operator/=(const float& other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        this->w /= other;
        return *this;
    }
    bool Vec4::operator==(const Vec4& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
    }
    bool Vec4::operator!=(const Vec4& other) const
    {
        return this->x != other.x || this->y != other.y || this->z != other.z || this->w != other.w;
    }
    Vec4 Vec4::operator*(const Mat4x4& other) const
    {
        return {this->x * other.m00 + this->y * other.m10 + this->z * other.m20 + this->w * other.m30, this->x * other.m01 + this->y * other.m11 + this->z * other.m21 + this->w * other.m31, this->x * other.m02 + this->y * other.m12 + this->z * other.m22 + this->w * other.m32, this->x * other.m03 + this->y * other.m13 + this->z * other.m23 + this->w * other.m33};
    }
    Vec4 Vec4::operator*=(const Mat4x4& other)
    {
        float x = this->x;
        float y = this->y;
        float z = this->z;
        this->x = x * other.m00 + y * other.m10 + z * other.m20 + this->w * other.m30;
        this->y = x * other.m01 + y * other.m11 + z * other.m21 + this->w * other.m31;
        this->z = x * other.m02 + y * other.m12 + z * other.m22 + this->w * other.m32;
        this->w = x * other.m03 + y * other.m13 + z * other.m23 + this->w * other.m33;
        return *this;
    }
    float Vec4::dot(const Vec4& other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
    }
    Vec4 Vec4::cross(const Vec4& other) const
    {
        return {this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x, 0.0};
    }
    float Vec4::length() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
    }
    Vec4 Vec4::normalized() const
    {
        float l = this->length();
        return {this->x / l, this->y / l, this->z / l, this->w / l};
    }
    Vec4 Vec4::rotate(float angle, const Vec4& axis) const
    {
        float s = sin(angle);
        float c = cos(angle);
        float oc = 1.0f - c;
        return {this->x * (oc * axis.x * axis.x + c) + this->y * (oc * axis.x * axis.y - axis.z * s) + this->z * (oc * axis.x * axis.z + axis.y * s), this->x * (oc * axis.x * axis.y + axis.z * s) + this->y * (oc * axis.y * axis.y + c) + this->z * (oc * axis.y * axis.z - axis.x * s), this->x * (oc * axis.x * axis.z - axis.y * s) + this->y * (oc * axis.y * axis.z + axis.x * s) + this->z * (oc * axis.z * axis.z + c), this->w};
    }
    Vec4 Vec4::lerp(const Vec4& other, float t) const
    {
        return {this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t, this->z + (other.z - this->z) * t, this->w + (other.w - this->w) * t};
    }
    Vec4 Vec4::slerp(const Vec4& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t, this->cross(other)) * (1.0f - t)) + (other * t);
    }
    Vec4 Vec4::direction_to(const Vec4& other) const
    {
        return (other - *this).normalized();
    }
    float Vec4::distance_to(const Vec4& other) const
    {
        return (other - *this).length();
    }
    float Vec4::angle_to(const Vec4& other) const
    {
        return atan2(other.y - this->y, other.x - this->x);
    }

    Vec4i::Vec4i()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        this->w = 0.0;
    }
    Vec4i::Vec4i(int x, int y, int z, int w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vec4i Vec4i::operator+(const Vec4i& other) const
    {
        return {this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w};
    }
    Vec4i Vec4i::operator-(const Vec4i& other) const
    {
        return {this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w};
    }
    Vec4i Vec4i::operator*(const Vec4i& other) const
    {
        return {this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w};
    }
    Vec4i Vec4i::operator*(const float& other) const
    {
        return {(int)((float)this->x * other), (int)((float)this->y * other), (int)((float)this->z * other), (int)((float)this->w * other)};
    }
    Vec4i Vec4i::operator/(const Vec4i& other) const
    {
        return {this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w};
    }
    Vec4i Vec4i::operator/(const float& other) const
    {
        return {(int)((float)this->x / other), (int)((float)this->y / other), (int)((float)this->z / other), (int)((float)this->w / other)};
    }
    Vec4i Vec4i::operator+=(const Vec4i& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }
    Vec4i Vec4i::operator-=(const Vec4i& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }
    Vec4i Vec4i::operator*=(const Vec4i& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        this->w *= other.w;
        return *this;
    }
    Vec4i Vec4i::operator*=(const float& other)
    {
        this->x *= (int)other;
        this->y *= (int)other;
        this->z *= (int)other;
        this->w *= (int)other;
        return *this;
    }
    Vec4i Vec4i::operator/=(const Vec4i& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        this->w /= other.w;
        return *this;
    }
    Vec4i Vec4i::operator/=(const float& other)
    {
        this->x /= (int)other;
        this->y /= (int)other;
        this->z /= (int)other;
        this->w /= (int)other;
        return *this;
    }
    bool Vec4i::operator==(const Vec4i& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
    }
    bool Vec4i::operator!=(const Vec4i& other) const
    {
        return this->x != other.x || this->y != other.y || this->z != other.z || this->w != other.w;
    }
    float Vec4i::dot(const Vec4i& other) const
    {
        return (float)this->x * (float)other.x + (float)this->y * (float)other.y + (float)this->z * (float)other.z + (float)this->w * (float)other.w;
    }
    Vec4i Vec4i::cross(const Vec4i& other) const
    {
        return {(int)((float)this->y * (float)other.z - (float)this->z * (float)other.y), (int)((float)this->z * (float)other.x - (float)this->x * (float)other.z), (int)((float)this->x * (float)other.y - (float)this->y * (float)other.x), 0};
    }
    float Vec4i::length() const
    {
        return sqrt((float)this->x * (float)this->x + (float)this->y * (float)this->y + (float)this->z * (float)this->z + (float)this->w * (float)this->w);
    }
    Vec4i Vec4i::normalized() const
    {
        float l = this->length();
        return {(int)((float)this->x / l), (int)((float)this->y / l), (int)((float)this->z / l), (int)((float)this->w / l)};
    }
    Vec4i Vec4i::rotate(float angle, const Vec4i& axis) const
    {
        return {(int)((float)this->x * cos(angle) - (float)this->y * sin(angle)), (int)((float)this->x * sin(angle) + (float)this->y * cos(angle)), (int)((float)this->z), (int)((float)this->w)};
    }
    Vec4i Vec4i::lerp(const Vec4i& other, float t) const
    {
        return {(int)((float)this->x + ((float)other.x - (float)this->x) * t), (int)((float)this->y + ((float)other.y - (float)this->y) * t), (int)((float)this->z + ((float)other.z - (float)this->z) * t), (int)((float)this->w + ((float)other.w - (float)this->w) * t)};
    }
    Vec4i Vec4i::slerp(const Vec4i& other, float t) const
    {
        float theta = acos(this->dot(other) / (this->length() * other.length()));
        return (this->rotate(theta * t, this->cross(other)) * (1.0f - t)) + (other * t);
    }
    Vec4i Vec4i::direction_to(const Vec4i& other) const
    {
        return (other - *this).normalized();
    }
    float Vec4i::distance_to(const Vec4i& other) const
    {
        return (other - *this).length();
    }
    float Vec4i::angle_to(const Vec4i& other) const
    {
        return atan2((float)other.y - (float)this->y, (float)other.x - (float)this->x);
    }

    Mat2x2::Mat2x2()
    {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m10 = 0.0;
        this->m11 = 1.0;
    }
    Mat2x2::Mat2x2(float m00, float m01, float m10, float m11)
    {
        this->m00 = m00;
        this->m01 = m01;
        this->m10 = m10;
        this->m11 = m11;
    }
    Mat2x2 Mat2x2::operator+(const Mat2x2& other) const
    {
        return {this->m00 + other.m00, this->m01 + other.m01, this->m10 + other.m10, this->m11 + other.m11};
    }
    Mat2x2 Mat2x2::operator-(const Mat2x2& other) const
    {
        return {this->m00 - other.m00, this->m01 - other.m01, this->m10 - other.m10, this->m11 - other.m11};
    }
    Mat2x2 Mat2x2::operator*(const Mat2x2& other) const
    {
        return {this->m00 * other.m00 + this->m01 * other.m10, this->m00 * other.m01 + this->m01 * other.m11, this->m10 * other.m00 + this->m11 * other.m10, this->m10 * other.m01 + this->m11 * other.m11};
    }
    Mat2x2 Mat2x2::operator*(const float& other) const
    {
        return {this->m00 * other, this->m01 * other, this->m10 * other, this->m11 * other};
    }
    Mat2x2 Mat2x2::operator/(const Mat2x2& other) const
    {
        return {this->m00 / other.m00, this->m01 / other.m01, this->m10 / other.m10, this->m11 / other.m11};
    }
    Mat2x2 Mat2x2::operator/(const float& other) const
    {
        return {this->m00 / other, this->m01 / other, this->m10 / other, this->m11 / other};
    }
    Mat2x2 Mat2x2::operator+=(const Mat2x2& other)
    {
        this->m00 += other.m00;
        this->m01 += other.m01;
        this->m10 += other.m10;
        this->m11 += other.m11;
        return *this;
    }
    Mat2x2 Mat2x2::operator-=(const Mat2x2& other)
    {
        this->m00 -= other.m00;
        this->m01 -= other.m01;
        this->m10 -= other.m10;
        this->m11 -= other.m11;
        return *this;
    }
    Mat2x2 Mat2x2::operator*=(const Mat2x2& other)
    {
        this->m00 = this->m00 * other.m00 + this->m01 * other.m10;
        this->m01 = this->m00 * other.m01 + this->m01 * other.m11;
        this->m10 = this->m10 * other.m00 + this->m11 * other.m10;
        this->m11 = this->m10 * other.m01 + this->m11 * other.m11;
        return *this;
    }
    Mat2x2 Mat2x2::operator*=(const float& other)
    {
        this->m00 *= other;
        this->m01 *= other;
        this->m10 *= other;
        this->m11 *= other;
        return *this;
    }
    Mat2x2 Mat2x2::operator/=(const Mat2x2& other)
    {
        this->m00 /= other.m00;
        this->m01 /= other.m01;
        this->m10 /= other.m10;
        this->m11 /= other.m11;
        return *this;
    }
    Mat2x2 Mat2x2::operator/=(const float& other)
    {
        this->m00 /= other;
        this->m01 /= other;
        this->m10 /= other;
        this->m11 /= other;
        return *this;
    }
    bool Mat2x2::operator==(const Mat2x2& other) const
    {
        return this->m00 == other.m00 && this->m01 == other.m01 && this->m10 == other.m10 && this->m11 == other.m11;
    }
    bool Mat2x2::operator!=(const Mat2x2& other) const
    {
        return this->m00 != other.m00 || this->m01 != other.m01 || this->m10 != other.m10 || this->m11 != other.m11;
    }
    Mat2x2 Mat2x2::transpose() const
    {
        return {this->m00, this->m10, this->m01, this->m11};
    }
    Mat2x2 Mat2x2::inverse() const
    {
        float det = this->m00 * this->m11 - this->m01 * this->m10;
        if (det == 0.0)
        {
            return *this;
        }
        return {this->m11 / det, -this->m01 / det, -this->m10 / det, this->m00 / det};
    }
    float Mat2x2::determinant() const
    {
        return this->m00 * this->m11 - this->m01 * this->m10;
    }

    Mat3x3::Mat3x3()
    {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        this->m11 = 1.0;
        this->m12 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = 1.0;
    }
    Mat3x3::Mat3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
    {
        this->m00 = m00;
        this->m01 = m01;
        this->m02 = m02;
        this->m10 = m10;
        this->m11 = m11;
        this->m12 = m12;
        this->m20 = m20;
        this->m21 = m21;
        this->m22 = m22;
    }
    Mat3x3 Mat3x3::operator+(const Mat3x3& other) const {
        return {this->m00 + other.m00, this->m01 + other.m01, this->m02 + other.m02, this->m10 + other.m10,
                this->m11 + other.m11, this->m12 + other.m12, this->m20 + other.m20, this->m21 + other.m21,
                this->m22 + other.m22};
    }
    Mat3x3 Mat3x3::operator-(const Mat3x3& other) const {
        return {this->m00 - other.m00, this->m01 - other.m01, this->m02 - other.m02, this->m10 - other.m10,
                this->m11 - other.m11, this->m12 - other.m12, this->m20 - other.m20, this->m21 - other.m21,
                this->m22 - other.m22};
    }
    Mat3x3 Mat3x3::operator*(const Mat3x3& other) const {
        return {this->m00 * other.m00 + this->m01 * other.m10 + this->m02 * other.m20,
                this->m00 * other.m01 + this->m01 * other.m11 + this->m02 * other.m21,
                this->m00 * other.m02 + this->m01 * other.m12 + this->m02 * other.m22,
                this->m10 * other.m00 + this->m11 * other.m10 + this->m12 * other.m20,
                this->m10 * other.m01 + this->m11 * other.m11 + this->m12 * other.m21,
                this->m10 * other.m02 + this->m11 * other.m12 + this->m12 * other.m22,
                this->m20 * other.m00 + this->m21 * other.m10 + this->m22 * other.m20,
                this->m20 * other.m01 + this->m21 * other.m11 + this->m22 * other.m21,
                this->m20 * other.m02 + this->m21 * other.m12 + this->m22 * other.m22};
    }
    Mat3x3 Mat3x3::operator*(const float& other) const {
        return {this->m00 * other, this->m01 * other, this->m02 * other, this->m10 * other, this->m11 * other,
                this->m12 * other, this->m20 * other, this->m21 * other, this->m22 * other};
    }
    Mat3x3 Mat3x3::operator/(const Mat3x3& other) const {
        return {this->m00 / other.m00, this->m01 / other.m01, this->m02 / other.m02, this->m10 / other.m10,
                this->m11 / other.m11, this->m12 / other.m12, this->m20 / other.m20, this->m21 / other.m21,
                this->m22 / other.m22};
    }
    Mat3x3 Mat3x3::operator/(const float& other) const {
        return {this->m00 / other, this->m01 / other, this->m02 / other, this->m10 / other, this->m11 / other,
                this->m12 / other, this->m20 / other, this->m21 / other, this->m22 / other};
    }
    Mat3x3 Mat3x3::operator+=(const Mat3x3& other)
    {
        this->m00 += other.m00;
        this->m01 += other.m01;
        this->m02 += other.m02;
        this->m10 += other.m10;
        this->m11 += other.m11;
        this->m12 += other.m12;
        this->m20 += other.m20;
        this->m21 += other.m21;
        this->m22 += other.m22;
        return *this;
    }
    Mat3x3 Mat3x3::operator-=(const Mat3x3& other)
    {
        this->m00 -= other.m00;
        this->m01 -= other.m01;
        this->m02 -= other.m02;
        this->m10 -= other.m10;
        this->m11 -= other.m11;
        this->m12 -= other.m12;
        this->m20 -= other.m20;
        this->m21 -= other.m21;
        this->m22 -= other.m22;
        return *this;
    }
    Mat3x3 Mat3x3::operator*=(const Mat3x3& other)
    {
        this->m00 = this->m00 * other.m00 + this->m01 * other.m10 + this->m02 * other.m20;
        this->m01 = this->m00 * other.m01 + this->m01 * other.m11 + this->m02 * other.m21;
        this->m02 = this->m00 * other.m02 + this->m01 * other.m12 + this->m02 * other.m22;
        this->m10 = this->m10 * other.m00 + this->m11 * other.m10 + this->m12 * other.m20;
        this->m11 = this->m10 * other.m01 + this->m11 * other.m11 + this->m12 * other.m21;
        this->m12 = this->m10 * other.m02 + this->m11 * other.m12 + this->m12 * other.m22;
        this->m20 = this->m20 * other.m00 + this->m21 * other.m10 + this->m22 * other.m20;
        this->m21 = this->m20 * other.m01 + this->m21 * other.m11 + this->m22 * other.m21;
        this->m22 = this->m20 * other.m02 + this->m21 * other.m12 + this->m22 * other.m22;
        return *this;
    }
    Mat3x3 Mat3x3::operator*=(const float& other)
    {
        this->m00 *= other;
        this->m01 *= other;
        this->m02 *= other;
        this->m10 *= other;
        this->m11 *= other;
        this->m12 *= other;
        this->m20 *= other;
        this->m21 *= other;
        this->m22 *= other;
        return *this;
    }
    Mat3x3 Mat3x3::operator/=(const Mat3x3& other)
    {
        this->m00 /= other.m00;
        this->m01 /= other.m01;
        this->m02 /= other.m02;
        this->m10 /= other.m10;
        this->m11 /= other.m11;
        this->m12 /= other.m12;
        this->m20 /= other.m20;
        this->m21 /= other.m21;
        this->m22 /= other.m22;
        return *this;
    }
    Mat3x3 Mat3x3::operator/=(const float& other) {
        this->m00 /= other;
        this->m01 /= other;
        this->m02 /= other;
        this->m10 /= other;
        this->m11 /= other;
        this->m12 /= other;
        this->m20 /= other;
        this->m21 /= other;
        this->m22 /= other;
        return *this;
    }
    bool Mat3x3::operator==(const Mat3x3& other) const {
        return this->m00 == other.m00 && this->m01 == other.m01 && this->m02 == other.m02 && this->m10 == other.m10 &&
               this->m11 == other.m11 && this->m12 == other.m12 && this->m20 == other.m20 && this->m21 == other.m21 &&
               this->m22 == other.m22;
    }
    bool Mat3x3::operator!=(const Mat3x3& other) const {
        return this->m00 != other.m00 || this->m01 != other.m01 || this->m02 != other.m02 || this->m10 != other.m10 ||
               this->m11 != other.m11 || this->m12 != other.m12 || this->m20 != other.m20 || this->m21 != other.m21 ||
               this->m22 != other.m22;
    }
    Mat3x3 Mat3x3::transpose() const {
        return {this->m00, this->m10, this->m20, this->m01, this->m11, this->m21, this->m02, this->m12, this->m22};
    }
    Mat3x3 Mat3x3::inverse() const {
        float det = this->m00 * (this->m11 * this->m22 - this->m12 * this->m21) -
                    this->m01 * (this->m10 * this->m22 - this->m12 * this->m20) +
                    this->m02 * (this->m10 * this->m21 - this->m11 * this->m20);
        if (det == 0.0) {
            return *this;
        }
        return {(this->m11 * this->m22 - this->m12 * this->m21) / det,
                (this->m02 * this->m21 - this->m01 * this->m22) / det,
                (this->m01 * this->m12 - this->m02 * this->m11) / det,
                (this->m12 * this->m20 - this->m10 * this->m22) / det,
                (this->m00 * this->m22 - this->m02 * this->m20) / det,
                (this->m02 * this->m10 - this->m00 * this->m12) / det,
                (this->m10 * this->m21 - this->m11 * this->m20) / det,
                (this->m01 * this->m20 - this->m00 * this->m21) / det,
                (this->m00 * this->m11 - this->m01 * this->m10) / det};
    }
    float Mat3x3::determinant() const {
        return this->m00 * (this->m11 * this->m22 - this->m12 * this->m21) -
               this->m01 * (this->m10 * this->m22 - this->m12 * this->m20) +
               this->m02 * (this->m10 * this->m21 - this->m11 * this->m20);
    }

    Mat4x4::Mat4x4()
    {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m03 = 0.0;
        this->m10 = 0.0;
        this->m11 = 1.0;
        this->m12 = 0.0;
        this->m13 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = 1.0;
        this->m23 = 0.0;
        this->m30 = 0.0;
        this->m31 = 0.0;
        this->m32 = 0.0;
        this->m33 = 1.0;
    }
    Mat4x4::Mat4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
    {
        this->m00 = m00;
        this->m01 = m01;
        this->m02 = m02;
        this->m03 = m03;
        this->m10 = m10;
        this->m11 = m11;
        this->m12 = m12;
        this->m13 = m13;
        this->m20 = m20;
        this->m21 = m21;
        this->m22 = m22;
        this->m23 = m23;
        this->m30 = m30;
        this->m31 = m31;
        this->m32 = m32;
        this->m33 = m33;
    }
    Mat4x4 Mat4x4::operator+(const Mat4x4& other) const {
        return {this->m00 + other.m00, this->m01 + other.m01, this->m02 + other.m02, this->m03 + other.m03,
                this->m10 + other.m10, this->m11 + other.m11, this->m12 + other.m12, this->m13 + other.m13,
                this->m20 + other.m20, this->m21 + other.m21, this->m22 + other.m22, this->m23 + other.m23,
                this->m30 + other.m30, this->m31 + other.m31, this->m32 + other.m32, this->m33 + other.m33};
    }
    Mat4x4 Mat4x4::operator-(const Mat4x4& other) const {
        return {this->m00 - other.m00, this->m01 - other.m01, this->m02 - other.m02, this->m03 - other.m03,
                this->m10 - other.m10, this->m11 - other.m11, this->m12 - other.m12, this->m13 - other.m13,
                this->m20 - other.m20, this->m21 - other.m21, this->m22 - other.m22, this->m23 - other.m23,
                this->m30 - other.m30, this->m31 - other.m31, this->m32 - other.m32, this->m33 - other.m33};
    }
    Mat4x4 Mat4x4::operator*(const Mat4x4& other) const {
        return {this->m00 * other.m00 + this->m01 * other.m10 + this->m02 * other.m20 + this->m03 * other.m30,
                this->m00 * other.m01 + this->m01 * other.m11 + this->m02 * other.m21 + this->m03 * other.m31,
                this->m00 * other.m02 + this->m01 * other.m12 + this->m02 * other.m22 + this->m03 * other.m32,
                this->m00 * other.m03 + this->m01 * other.m13 + this->m02 * other.m23 + this->m03 * other.m33,
                this->m10 * other.m00 + this->m11 * other.m10 + this->m12 * other.m20 + this->m13 * other.m30,
                this->m10 * other.m01 + this->m11 * other.m11 + this->m12 * other.m21 + this->m13 * other.m31,
                this->m10 * other.m02 + this->m11 * other.m12 + this->m12 * other.m22 + this->m13 * other.m32,
                this->m10 * other.m03 + this->m11 * other.m13 + this->m12 * other.m23 + this->m13 * other.m33,
                this->m20 * other.m00 + this->m21 * other.m10 + this->m22 * other.m20 + this->m23 * other.m30,
                this->m20 * other.m01 + this->m21 * other.m11 + this->m22 * other.m21 + this->m23 * other.m31,
                this->m20 * other.m02 + this->m21 * other.m12 + this->m22 * other.m22 + this->m23 * other.m32,
                this->m20 * other.m03 + this->m21 * other.m13 + this->m22 * other.m23 + this->m23 * other.m33,
                this->m30 * other.m00 + this->m31 * other.m10 + this->m32 * other.m20 + this->m33 * other.m30,
                this->m30 * other.m01 + this->m31 * other.m11 + this->m32 * other.m21 + this->m33 * other.m31,
                this->m30 * other.m02 + this->m31 * other.m12 + this->m32 * other.m22 + this->m33 * other.m32,
                this->m30 * other.m03 + this->m31 * other.m13 + this->m32 * other.m23 + this->m33 * other.m33};
    }
    Mat4x4 Mat4x4::operator*(const float& other) const {
        return {this->m00 * other, this->m01 * other, this->m02 * other, this->m03 * other,
                this->m10 * other, this->m11 * other, this->m12 * other, this->m13 * other,
                this->m20 * other, this->m21 * other, this->m22 * other, this->m23 * other,
                this->m30 * other, this->m31 * other, this->m32 * other, this->m33 * other};
    }
    Mat4x4 Mat4x4::operator/(const Mat4x4& other) const {
        return {this->m00 / other.m00, this->m01 / other.m01, this->m02 / other.m02, this->m03 / other.m03,
                this->m10 / other.m10, this->m11 / other.m11, this->m12 / other.m12, this->m13 / other.m13,
                this->m20 / other.m20, this->m21 / other.m21, this->m22 / other.m22, this->m23 / other.m23,
                this->m30 / other.m30, this->m31 / other.m31, this->m32 / other.m32, this->m33 / other.m33};
    }
    Mat4x4 Mat4x4::operator/(const float& other) const {
        return {this->m00 / other, this->m01 / other, this->m02 / other, this->m03 / other,
                this->m10 / other, this->m11 / other, this->m12 / other, this->m13 / other,
                this->m20 / other, this->m21 / other, this->m22 / other, this->m23 / other,
                this->m30 / other, this->m31 / other, this->m32 / other, this->m33 / other};
    }
    Mat4x4 Mat4x4::operator+=(const Mat4x4& other)
    {
        this->m00 += other.m00;
        this->m01 += other.m01;
        this->m02 += other.m02;
        this->m03 += other.m03;
        this->m10 += other.m10;
        this->m11 += other.m11;
        this->m12 += other.m12;
        this->m13 += other.m13;
        this->m20 += other.m20;
        this->m21 += other.m21;
        this->m22 += other.m22;
        this->m23 += other.m23;
        this->m30 += other.m30;
        this->m31 += other.m31;
        this->m32 += other.m32;
        this->m33 += other.m33;
        return *this;
    }
    Mat4x4 Mat4x4::operator-=(const Mat4x4& other)
    {
        this->m00 -= other.m00;
        this->m01 -= other.m01;
        this->m02 -= other.m02;
        this->m03 -= other.m03;
        this->m10 -= other.m10;
        this->m11 -= other.m11;
        this->m12 -= other.m12;
        this->m13 -= other.m13;
        this->m20 -= other.m20;
        this->m21 -= other.m21;
        this->m22 -= other.m22;
        this->m23 -= other.m23;
        this->m30 -= other.m30;
        this->m31 -= other.m31;
        this->m32 -= other.m32;
        this->m33 -= other.m33;
        return *this;
    }
    Mat4x4 Mat4x4::operator*=(const Mat4x4& other) {
        this->m00 = this->m00 * other.m00 + this->m01 * other.m10 + this->m02 * other.m20 + this->m03 * other.m30;
        this->m01 = this->m00 * other.m01 + this->m01 * other.m11 + this->m02 * other.m21 + this->m03 * other.m31;
        this->m02 = this->m00 * other.m02 + this->m01 * other.m12 + this->m02 * other.m22 + this->m03 * other.m32;
        this->m03 = this->m00 * other.m03 + this->m01 * other.m13 + this->m02 * other.m23 + this->m03 * other.m33;
        this->m10 = this->m10 * other.m00 + this->m11 * other.m10 + this->m12 * other.m20 + this->m13 * other.m30;
        this->m11 = this->m10 * other.m01 + this->m11 * other.m11 + this->m12 * other.m21 + this->m13 * other.m31;
        this->m12 = this->m10 * other.m02 + this->m11 * other.m12 + this->m12 * other.m22 + this->m13 * other.m32;
        this->m13 = this->m10 * other.m03 + this->m11 * other.m13 + this->m12 * other.m23 + this->m13 * other.m33;
        this->m20 = this->m20 * other.m00 + this->m21 * other.m10 + this->m22 * other.m20 + this->m23 * other.m30;
        this->m21 = this->m20 * other.m01 + this->m21 * other.m11 + this->m22 * other.m21 + this->m23 * other.m31;
        this->m22 = this->m20 * other.m02 + this->m21 * other.m12 + this->m22 * other.m22 + this->m23 * other.m32;
        this->m23 = this->m20 * other.m03 + this->m21 * other.m13 + this->m22 * other.m23 + this->m23 * other.m33;
        this->m30 = this->m30 * other.m00 + this->m31 * other.m10 + this->m32 * other.m20 + this->m33 * other.m30;
        this->m31 = this->m30 * other.m01 + this->m31 * other.m11 + this->m32 * other.m21 + this->m33 * other.m31;
        this->m32 = this->m30 * other.m02 + this->m31 * other.m12 + this->m32 * other.m22 + this->m33 * other.m32;
        this->m33 = this->m30 * other.m03 + this->m31 * other.m13 + this->m32 * other.m23 + this->m33 * other.m33;
        return *this;
    }
    Mat4x4 Mat4x4::operator*=(const float& other) {
        this->m00 *= other;
        this->m01 *= other;
        this->m02 *= other;
        this->m03 *= other;
        this->m10 *= other;
        this->m11 *= other;
        this->m12 *= other;
        this->m13 *= other;
        this->m20 *= other;
        this->m21 *= other;
        this->m22 *= other;
        this->m23 *= other;
        this->m30 *= other;
        this->m31 *= other;
        this->m32 *= other;
        this->m33 *= other;
        return *this;
    }
    Mat4x4 Mat4x4::operator/=(const Mat4x4& other) {
        this->m00 /= other.m00;
        this->m01 /= other.m01;
        this->m02 /= other.m02;
        this->m03 /= other.m03;
        this->m10 /= other.m10;
        this->m11 /= other.m11;
        this->m12 /= other.m12;
        this->m13 /= other.m13;
        this->m20 /= other.m20;
        this->m21 /= other.m21;
        this->m22 /= other.m22;
        this->m23 /= other.m23;
        this->m30 /= other.m30;
        this->m31 /= other.m31;
        this->m32 /= other.m32;
        this->m33 /= other.m33;
        return *this;
    }
    Mat4x4 Mat4x4::operator/=(const float& other) {
        this->m00 /= other;
        this->m01 /= other;
        this->m02 /= other;
        this->m03 /= other;
        this->m10 /= other;
        this->m11 /= other;
        this->m12 /= other;
        this->m13 /= other;
        this->m20 /= other;
        this->m21 /= other;
        this->m22 /= other;
        this->m23 /= other;
        this->m30 /= other;
        this->m31 /= other;
        this->m32 /= other;
        this->m33 /= other;
        return *this;
    }
    bool Mat4x4::operator==(const Mat4x4& other) const {
        return this->m00 == other.m00 && this->m01 == other.m01 && this->m02 == other.m02 && this->m03 == other.m03 &&
               this->m10 == other.m10 && this->m11 == other.m11 && this->m12 == other.m12 && this->m13 == other.m13 &&
               this->m20 == other.m20 && this->m21 == other.m21 && this->m22 == other.m22 && this->m23 == other.m23 &&
               this->m30 == other.m30 && this->m31 == other.m31 && this->m32 == other.m32 && this->m33 == other.m33;
    }
    bool Mat4x4::operator!=(const Mat4x4& other) const {
        return this->m00 != other.m00 || this->m01 != other.m01 || this->m02 != other.m02 || this->m03 != other.m03 ||
               this->m10 != other.m10 || this->m11 != other.m11 || this->m12 != other.m12 || this->m13 != other.m13 ||
               this->m20 != other.m20 || this->m21 != other.m21 || this->m22 != other.m22 || this->m23 != other.m23 ||
               this->m30 != other.m30 || this->m31 != other.m31 || this->m32 != other.m32 || this->m33 != other.m33;
    }
    Mat4x4 Mat4x4::transpose() const {
        return {this->m00, this->m10, this->m20, this->m30, this->m01, this->m11, this->m21, this->m31,
                this->m02, this->m12, this->m22, this->m32, this->m03, this->m13, this->m23, this->m33};
    }
    Mat4x4 Mat4x4::inverse() const {
        float det = this->m00 * (this->m11 * (this->m22 * this->m33 - this->m23 * this->m32) -
                                 this->m12 * (this->m21 * this->m33 - this->m23 * this->m31) +
                                 this->m13 * (this->m21 * this->m32 - this->m22 * this->m31)) -
                    this->m01 * (this->m10 * (this->m22 * this->m33 - this->m23 * this->m32) -
                                 this->m12 * (this->m20 * this->m33 - this->m23 * this->m30) +
                                 this->m13 * (this->m20 * this->m32 - this->m22 * this->m30)) +
                    this->m02 * (this->m10 * (this->m21 * this->m33 - this->m23 * this->m31) -
                                 this->m11 * (this->m20 * this->m33 - this->m23 * this->m30) +
                                 this->m13 * (this->m20 * this->m31 - this->m21 * this->m30)) -
                    this->m03 * (this->m10 * (this->m21 * this->m32 - this->m22 * this->m31) -
                                 this->m11 * (this->m20 * this->m32 - this->m22 * this->m30) +
                                 this->m12 * (this->m20 * this->m31 - this->m21 * this->m30));
        if (det == 0.0) {
            return *this;
        }
        return {(this->m11 * (this->m22 * this->m33 - this->m23 * this->m32) -
                 this->m12 * (this->m21 * this->m33 - this->m23 * this->m31) +
                 this->m13 * (this->m21 * this->m32 - this->m22 * this->m31)) / det,
                (this->m02 * (this->m23 * this->m31 - this->m21 * this->m33) -
                 this->m03 * (this->m22 * this->m31 - this->m21 * this->m32) +
                 this->m01 * (this->m22 * this->m33 - this->m23 * this->m32)) / det,
                (this->m03 * (this->m11 * this->m22 - this->m12 * this->m21) -
                 this->m01 * (this->m13 * this->m22 - this->m12 * this->m23) +
                 this->m02 * (this->m13 * this->m21 - this->m11 * this->m23)) / det,
                (this->m01 * (this->m13 * this->m32 - this->m12 * this->m33) -
                 this->m02 * (this->m11 * this->m33 - this->m13 * this->m31) +
                 this->m03 * (this->m12 * this->m31 - this->m11 * this->m32)) / det,
                (this->m12 * (this->m20 * this->m33 - this->m23 * this->m30) -
                 this->m13 * (this->m20 * this->m32 - this->m22 * this->m30) +
                 this->m10 * (this->m22 * this->m33 - this->m23 * this->m32)) / det,
                (this->m03 * (this->m20 * this->m32 - this->m22 * this->m30) -
                 this->m00 * (this->m22 * this->m33 - this->m23 * this->m32) +
                 this->m02 * (this->m23 * this->m30 - this->m20 * this->m33)) / det,
                (this->m00 * (this->m13 * this->m32 - this->m12 * this->m33) -
                 this->m02 * (this->m10 * this->m33 - this->m13 * this->m30) +
                 this->m03 * (this->m12 * this->m30 - this->m10 * this->m32)) / det,
                (this->m02 * (this->m10 * this->m33 - this->m13 * this->m30) -
                 this->m03 * (this->m10 * this->m32 - this->m12 * this->m30) +
                 this->m00 * (this->m12 * this->m33 - this->m13 * this->m32)) / det,
                (this->m13 * (this->m20 * this->m31 - this->m21 * this->m30) -
                 this->m10 * (this->m23 * this->m31 - this->m21 * this->m33) +
                 this->m11 * (this->m23 * this->m30 - this->m20 * this->m33)) / det,
                (this->m00 * (this->m21 * this->m33 - this->m23 * this->m31) -
                 this->m01 * (this->m20 * this->m33 - this->m23 * this->m30) +
                 this->m03 * (this->m20 * this->m31 - this->m21 * this->m30)) / det,
                (this->m01 * (this->m10 * this->m33 - this->m13 * this->m30) -
                 this->m03 * (this->m10 * this->m31 - this->m11 * this->m30) +
                 this->m00 * (this->m13 * this->m31 - this->m11 * this->m33)) / det,
                (this->m03 * (this->m10 * this->m21 - this->m11 * this->m20) -
                 this->m00 * (this->m13 * this->m21 - this->m11 * this->m23) +
                 this->m01 * (this->m13 * this->m20 - this->m10 * this->m23)) / det,
                (this->m11 * (this->m20 * this->m32 - this->m22 * this->m30) -
                 this->m10 * (this->m21 * this->m32 - this->m22 * this->m31) +
                 this->m12 * (this->m21 * this->m30 - this->m20 * this->m31)) / det,
                (this->m00 * (this->m22 * this->m31 - this->m21 * this->m32) -
                 this->m01 * (this->m20 * this->m32 - this->m22 * this->m30) +
                 this->m02 * (this->m21 * this->m30 - this->m20 * this->m31)) / det,
                (this->m01 * (this->m10 * this->m32 - this->m12 * this->m30) -
                 this->m02 * (this->m10 * this->m31 - this->m11 * this->m30) +
                 this->m00 * (this->m12 * this->m31 - this->m11 * this->m32)) / det,
                (this->m02 * (this->m11 * this->m20 - this->m10 * this->m21) -
                 this->m00 * (this->m12 * this->m21 - this->m11 * this->m22) +
                 this->m01 * (this->m10 * this->m22 - this->m12 * this->m20)) / det};
    }

}