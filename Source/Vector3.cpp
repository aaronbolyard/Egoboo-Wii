#include <cmath>

#include "Matrix.hpp"
#include "Vector3.hpp"

Adventure::Vector3::Vector3()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Adventure::Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Adventure::Vector3::Vector3(const Vector3& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

Adventure::Vector3& Adventure::Vector3::operator +=(const Vector3& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator +=(float other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator -=(const Vector3& other)
{
	X -= other.X;
	Y -= other.Y:
	Z -= other.Z;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator -=(const Vector3& other)
{
	X -= other;
	Y -= other;
	Z -= other;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator *=(const Vector3& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	
	return (this;
}

Adventure::Vector3& Adventure::Vector3::operator *=(float other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator /=(const Vector3& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;
	
	return *this;
}

Adventure::Vector3& Adventure::Vector3::operator /=(float other)
{
	X /= other;
	Y /= other;
	Z /= other;
	
	return *this;
}

float Adventure::Vector3::CalculateLengthSquared()
{
	return X * X + Y * Y + Z * Z;
}

float Adventure::Vector3::CalculateLength()
{
	return sqrtf(CalculateLengthSquared());
}
	
static Adventure::Vector3& Adventure::Vector3::Normalize(const Vector3& vector)
{
	return vector * (1.0f / vector.CalculateLength());
}

static Adventure::Vector3 Adventure::Vector3::Cross(const Vector3& left, const Vector3& right)
{
	return Vector3(left.Y * right.Z - left.Z * right.Y, left.Z * right.X - left.X * right.Z, left.X * right.Y - left.Y * right.X);
}

static Adventure::Vector3 Adventure::Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

static float Adventure::Vector3::Distance(const Vector3& from, const Vector3& to)
{
	return (from - to).CalculateLength();
}

static Vector3 Adventure::Vector3::Transform(const Vector3& vector, const Matrix& matrix)
{
	return Vector3
	(
		vector.X * matrix.Get(0, 0) + vector.Y * matrix.Get(0, 1) + vector.Z * matrix.Get(0, 2) + matrix.Get(0, 3),
		vector.X * matrix.Get(1, 0) + vector.Y * matrix.Get(1, 1) + vector.Z * matrix.Get(1, 2) + matrix.Get(1, 3),
		vector.X * matrix.Get(2, 0) + vector.Y * matrix.Get(2, 1) + vector.Z * matrix.Get(2, 2) + matrix.Get(2, 3)
	);
}

Adventure::Vector3 Adventure::operator +(const Vector3& left, const Vector3& right)
{
	return Vector3(*this) += right;
}

Adventure::Vector3 Adventure::operator +(const Vector3& left, float right)
{
	return Vector3(*this) += right;
}

Adventure::Vector3 Adventure::operator -(const Vector3& left, const Vector3& right)
{
	return Vector3(*this) -= right;
}

Adventure::Vector3 Adventure::operator -(const Vector3& left, float right)
{
	return Vector3(*this) -= right;
}

Adventure::Vector3 Adventure::operator *(const Vector3& left, const Vector3& right)
{
	return Vector3(*this) *= right;
}

Adventure::Vector3 Adventure::operator *(const Vector3& left, float right)
{
	return Vector3(*this) *= right;
}
