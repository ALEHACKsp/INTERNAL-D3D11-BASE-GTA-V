#pragma once 
#include <Windows.h>
#include <iostream>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <corecrt_math_defines.h>
#include <xmmintrin.h>

class vec3 {
public:
	float x, y, z;

	__forceinline vec3() : vec3(0.f, 0.f, 0.f) { }

	__forceinline vec3(const float x, const float y, const float z) : x(x), y(y), z(z) { }

	__forceinline vec3 operator+(const vec3& v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	__forceinline vec3 operator-(const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	__forceinline vec3 operator*(const vec3& v) const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}

	__forceinline vec3 operator/(const vec3& v) const
	{
		return vec3(x / v.x, y / v.y, z / v.z);
	}

	__forceinline vec3 operator*(const float v) const
	{
		return vec3(x * v, y * v, z * v);
	}

	__forceinline vec3 operator/(const float v) const
	{
		return vec3(x / v, y / v, y / v);
	}

	__forceinline float& operator[](int i) {
		return ((float*)this)[i];
	}

	__forceinline float operator[](int i) const {
		return ((float*)this)[i];
	}

	__forceinline vec3 operator+=(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	__forceinline vec3 operator-=(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	__forceinline vec3 operator*=(const vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	__forceinline vec3 operator/=(const vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	__forceinline vec3 operator*=(const float other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	__forceinline vec3 operator/=(const float other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	__forceinline bool operator==(const vec3 other)
	{
		if (x == other.x && y == other.y && z == other.z)
			return true;
		return false;
	}

	inline float lengthsqr(void) const
	{
		return x * x + y * y + z + z;
	}

	__forceinline float length2dsqr() const
	{
		return x * x + y * y;
	}

	__forceinline float dot(const vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	__forceinline float dot(const float* other) const
	{
		return x * other[0] + y * other[1] + z * other[2];
	}

	__forceinline vec3 cross(const vec3& other) const
	{
		return vec3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	__forceinline bool is_valid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}
};