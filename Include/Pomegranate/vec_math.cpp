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

    Mat::Mat()
    {
        this->rows = 0;
        this->cols = 0;
    }
    Mat::Mat(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;
        this->data = std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.0));
    }
    Mat::Mat(int rows, int cols, float** data)
    {
        this->rows = rows;
        this->cols = cols;
        this->data = std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.0));
    }
    Mat::Mat(const Mat& other)
    {
        this->rows = other.rows;
        this->cols = other.cols;
        this->data = other.data;
    }
    Mat::~Mat()
    {

    }
    Mat Mat::operator+(const Mat& other) const
    {
        Mat result(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
    Mat Mat::operator-(const Mat& other) const
    {
        Mat result(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->data[i][j] - other.data[i][j];
            }
        }
        return result;
    }
    Mat Mat::operator*(const Mat& other) const
    {
        Mat result(this->rows, other.cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < other.cols; j++)
            {
                for (int k = 0; k < this->cols; k++)
                {
                    result.data[i][j] += this->data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
    Mat Mat::operator*(const float& other) const
    {
        Mat result(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->data[i][j] * other;
            }
        }
        return result;
    }
    Mat Mat::operator/(const Mat& other) const
    {
        Mat result(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->data[i][j] / other.data[i][j];
            }
        }
        return result;
    }
    Mat Mat::operator/(const float& other) const
    {
        Mat result(this->rows, this->cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->data[i][j] / other;
            }
        }
        return result;
    }
    Mat Mat::operator+=(const Mat& other)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                this->data[i][j] += other.data[i][j];
            }
        }
        return *this;
    }
    Mat Mat::operator-=(const Mat& other)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                this->data[i][j] -= other.data[i][j];
            }
        }
        return *this;
    }
    Mat Mat::operator*=(const Mat& other)
    {
        Mat result(this->rows, other.cols);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < other.cols; j++)
            {
                for (int k = 0; k < this->cols; k++)
                {
                    result.data[i][j] += this->data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
    Mat Mat::operator*=(const float& other)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                this->data[i][j] *= other;
            }
        }
        return *this;
    }
    Mat Mat::operator/=(const Mat& other)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                this->data[i][j] /= other.data[i][j];
            }
        }
        return *this;
    }
    Mat Mat::operator/=(const float& other)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                this->data[i][j] /= other;
            }
        }
        return *this;
    }
    bool Mat::operator==(const Mat& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
        {
            return false;
        }
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                if (this->data[i][j] != other.data[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool Mat::operator!=(const Mat& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
        {
            return true;
        }
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                if (this->data[i][j] != other.data[i][j])
                {
                    return true;
                }
            }
        }
        return false;
    }
    Vec2 Mat::operator*(const Vec2& other) const
    {
        return {this->data[0][0] * other.x + this->data[0][1] * other.y, this->data[1][0] * other.x + this->data[1][1] * other.y};
    }
    Vec3 Mat::operator*(const Vec3& other) const
    {
        return {this->data[0][0] * other.x + this->data[0][1] * other.y + this->data[0][2] * other.z, this->data[1][0] * other.x + this->data[1][1] * other.y + this->data[1][2] * other.z, this->data[2][0] * other.x + this->data[2][1] * other.y + this->data[2][2] * other.z};
    }
    Vec4 Mat::operator*(const Vec4& other) const
    {
        return {this->data[0][0] * other.x + this->data[0][1] * other.y + this->data[0][2] * other.z + this->data[0][3] * other.w, this->data[1][0] * other.x + this->data[1][1] * other.y + this->data[1][2] * other.z + this->data[1][3] * other.w, this->data[2][0] * other.x + this->data[2][1] * other.y + this->data[2][2] * other.z + this->data[2][3] * other.w, this->data[3][0] * other.x + this->data[3][1] * other.y + this->data[3][2] * other.z + this->data[3][3] * other.w};
    }
    Vec2i Mat::operator*(const Vec2i& other) const
    {
        return {(int)(this->data[0][0] * other.x + this->data[0][1] * other.y), (int)(this->data[1][0] * other.x + this->data[1][1] * other.y)};
    }
    Vec3i Mat::operator*(const Vec3i& other) const
    {
        return {(int)(this->data[0][0] * other.x + this->data[0][1] * other.y + this->data[0][2] * other.z), (int)(this->data[1][0] * other.x + this->data[1][1] * other.y + this->data[1][2] * other.z), (int)(this->data[2][0] * other.x + this->data[2][1] * other.y + this->data[2][2] * other.z)};
    }
    Vec4i Mat::operator*(const Vec4i& other) const
    {
        return {(int)(this->data[0][0] * other.x + this->data[0][1] * other.y + this->data[0][2] * other.z + this->data[0][3] * other.w), (int)(this->data[1][0] * other.x + this->data[1][1] * other.y + this->data[1][2] * other.z + this->data[1][3] * other.w), (int)(this->data[2][0] * other.x + this->data[2][1] * other.y + this->data[2][2] * other.z + this->data[2][3] * other.w), (int)(this->data[3][0] * other.x + this->data[3][1] * other.y + this->data[3][2] * other.z + this->data[3][3] * other.w)};
    }
    Mat Mat::transpose() const
    {
        Mat result(this->cols, this->rows);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[j][i] = this->data[i][j];
            }
        }
        return result;
    }
    Mat Mat::inverse() const
    {
        if (this->rows != this->cols)
        {
            return Mat();
        }
        Mat result(this->rows, this->cols);
        float det = this->determinant();
        if (det == 0.0)
        {
            return Mat();
        }
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                result.data[i][j] = this->cofactor(i, j) / det;
            }
        }
        return result;
    }
    float Mat::determinant() const
    {
        if (this->rows != this->cols)
        {
            return 0.0;
        }
        if (this->rows == 2)
        {
            return this->data[0][0] * this->data[1][1] - this->data[0][1] * this->data[1][0];
        }
        float det = 0.0;
        for (int i = 0; i < this->rows; i++)
        {
            det += this->data[0][i] * this->cofactor(0, i);
        }
        return det;
    }
    float Mat::cofactor(int row, int col) const
    {
        Mat submatrix(this->rows - 1, this->cols - 1);
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                if (i != row && j != col)
                {
                    submatrix.data[i - (i > row ? 1 : 0)][j - (j > col ? 1 : 0)] = this->data[i][j];
                }
            }
        }
        return submatrix.determinant() * (row + col % 2 == 0 ? 1 : -1);
    }
    Mat Mat::identity(int size)
    {
        Mat result(size, size);
        for (int i = 0; i < size; i++)
        {
            result.data[i][i] = 1.0;
        }
        return result;
    }
    Mat Mat::zero(int rows, int cols)
    {
        Mat result(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = 0.0;
            }
        }
        return result;
    }
    Mat Mat::translate(const Pomegranate::Vec3 &translation)
    {
        Mat result(4, 4);
        result.data[0][0] = 1.0;
        result.data[1][1] = 1.0;
        result.data[2][2] = 1.0;
        result.data[3][3] = 1.0;
        result.data[0][3] = translation.x;
        result.data[1][3] = translation.y;
        result.data[2][3] = translation.z;
        return result;
    }
    Mat Mat::scale(const Pomegranate::Vec3 &scale)
    {
        Mat result(4, 4);
        result.data[0][0] = scale.x;
        result.data[1][1] = scale.y;
        result.data[2][2] = scale.z;
        result.data[3][3] = 1.0;
        return result;
    }
    Mat Mat::rotate(float angle, const Pomegranate::Vec3 &axis)
    {
        float s = sin(angle);
        float c = cos(angle);
        float oc = 1.0f - c;
        Mat result(4, 4);
        result.data[0][0] = oc * axis.x * axis.x + c;
        result.data[0][1] = oc * axis.x * axis.y - axis.z * s;
        result.data[0][2] = oc * axis.x * axis.z + axis.y * s;
        result.data[0][3] = 0.0;
        result.data[1][0] = oc * axis.x * axis.y + axis.z * s;
        result.data[1][1] = oc * axis.y * axis.y + c;
        result.data[1][2] = oc * axis.y * axis.z - axis.x * s;
        result.data[1][3] = 0.0;
        result.data[2][0] = oc * axis.x * axis.z - axis.y * s;
        result.data[2][1] = oc * axis.y * axis.z + axis.x * s;
        result.data[2][2] = oc * axis.z * axis.z + c;
        result.data[2][3] = 0.0;
        result.data[3][0] = 0.0;
        result.data[3][1] = 0.0;
        result.data[3][2] = 0.0;
        result.data[3][3] = 1.0;
        return result;
    }
    Mat Mat::look_at(const Pomegranate::Vec3 &eye, const Pomegranate::Vec3 &center, const Pomegranate::Vec3 &up)
    {
        Pomegranate::Vec3 f = (center - eye).normalized();
        Pomegranate::Vec3 r = f.cross(up).normalized();
        Pomegranate::Vec3 u = r.cross(f);
        Mat result(4, 4);
        result.data[0][0] = r.x;
        result.data[0][1] = r.y;
        result.data[0][2] = r.z;
        result.data[0][3] = -r.dot(eye);
        result.data[1][0] = u.x;
        result.data[1][1] = u.y;
        result.data[1][2] = u.z;
        result.data[1][3] = -u.dot(eye);
        result.data[2][0] = -f.x;
        result.data[2][1] = -f.y;
        result.data[2][2] = -f.z;
        result.data[2][3] = f.dot(eye);
        result.data[3][0] = 0.0;
        result.data[3][1] = 0.0;
        result.data[3][2] = 0.0;
        result.data[3][3] = 1.0;
        return result;
    }
    Mat Mat::perspective(float fov, float aspect, float near, float far)
    {
        fov = fov * 3.14159265358979323846 / 180.0;
        float f = 1.0f / tan(fov / 2.0f);
        Mat result(4, 4);
        result.data[0][0] = f / aspect;
        result.data[0][1] = 0.0;
        result.data[0][2] = 0.0;
        result.data[0][3] = 0.0;
        result.data[1][0] = 0.0;
        result.data[1][1] = f;
        result.data[1][2] = 0.0;
        result.data[1][3] = 0.0;
        result.data[2][0] = 0.0;
        result.data[2][1] = 0.0;
        result.data[2][2] = -((far) / (near - far));
        result.data[2][3] = -((far * near) / (near - far));
        result.data[3][0] = 0.0;
        result.data[3][1] = 0.0;
        result.data[3][2] = -1.0;
        result.data[3][3] = 0.0;
        return result;
    }
    Mat Mat::orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Mat result(4, 4);
        result.data[0][0] = 2.0f / (right - left);
        result.data[0][1] = 0.0;
        result.data[0][2] = 0.0;
        result.data[0][3] = -(right + left) / (right - left);
        result.data[1][0] = 0.0;
        result.data[1][1] = 2.0f / (top - bottom);
        result.data[1][2] = 0.0;
        result.data[1][3] = -(top + bottom) / (top - bottom);
        result.data[2][0] = 0.0;
        result.data[2][1] = 0.0;
        result.data[2][2] = -2.0f / (far - near);
        result.data[2][3] = -(far + near) / (far - near);
        result.data[3][0] = 0.0;
        result.data[3][1] = 0.0;
        result.data[3][2] = 0.0;
        result.data[3][3] = 1.0;
        return result;
    }
}