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
}