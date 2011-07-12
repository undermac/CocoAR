/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _MATH_H
#define _MATH_H

#define	_maxFloat		FLT_MAX
#define	_epsilon		FLT_EPSILON
#define _pi			3.14159265359f


#include <cmath>
#include <cfloat>
#include <cstddef>
#include <limits>

/// This function is used to ensure that a floating point number is
/// not a NaN or infinity.
inline bool IsValid(float x)
{
	if (x != x)
	{
		// NaN.
		return false;
	}

	float infinity = std::numeric_limits<float>::infinity();
	return -infinity < x && x < infinity;
}

/// This is a approximate yet fast inverse square-root.
inline float InvSqrt(float x)
{
	union
	{
		float x;
		int i;
	} convert;

	convert.x = x;
	float xhalf = 0.5f * x;
	convert.i = 0x5f3759df - (convert.i >> 1);
	x = convert.x;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

#define	Sqrt(x)	sqrtf(x)
#define	Atan2(y, x)	atan2f(y, x)

inline float Abs(float a)
{
	return a > 0.0f ? a : -a;
}

/// A 2D column vector.
struct Vec2
{
	/// Default constructor does nothing (for performance).
	Vec2() {}

	/// Construct using coordinates.
	Vec2(float x, float y) : x(x), y(y) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_) { x = x_; y = y_; }

	/// Negate this vector.
	Vec2 operator -() const { Vec2 v; v.Set(-x, -y); return v; }
	
	/// Read from and indexed element.
	float operator () (int i) const
	{
		return (&x)[i];
	}

	/// Write to an indexed element.
	float& operator () (int i)
	{
		return (&x)[i];
	}

	/// Add a vector to this vector.
	void operator += (const Vec2& v)
	{
		x += v.x; y += v.y;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const Vec2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float a)
	{
		x *= a; y *= a;
	}

	/// Get the length of this vector (the norm).
	float Length() const
	{
		return Sqrt(x * x + y * y);
	}

	/// Get the length squared. For performance, use this instead of
	/// Vec2::Length (if possible).
	float LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float Normalize()
	{
		float length = Length();
		if (length < _epsilon)
		{
			return 0.0f;
		}
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		//return IsValid(x) && IsValid(y);
		return true;
	}

	float x, y;
};

/// A 2D column vector with 3 elements.
struct Vec3
{
	/// Default constructor does nothing (for performance).
	Vec3() {}

	/// Construct using coordinates.
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

	/// Negate this vector.
	Vec3 operator -() const { Vec3 v; v.Set(-x, -y, -z); return v; }

	/// Add a vector to this vector.
	void operator += (const Vec3& v)
	{
		x += v.x; y += v.y; z += v.z;
	}

	/// Get the length of this vector (the norm).
	float Length() const
	{
		return Sqrt(x * x + y * y+ z * z);
	}

	/// Subtract a vector from this vector.
	void operator -= (const Vec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float s)
	{
		x *= s; y *= s; z *= s;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float Normalize()
	{
		float length = Length();
		if (length < _epsilon)
		{
			return 0.0f;
		}
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;

		return length;
	}

	float x, y, z;
};

/// A 2-by-2 matrix. Stored in column-major order.
struct Mat22
{
	/// The default constructor does nothing (for performance).
	Mat22() {}

	/// Construct this matrix using columns.
	Mat22(const Vec2& c1, const Vec2& c2)
	{
		col1 = c1;
		col2 = c2;
	}

	/// Construct this matrix using scalars.
	Mat22(float a11, float a12, float a21, float a22)
	{
		col1.x = a11; col1.y = a21;
		col2.x = a12; col2.y = a22;
	}

	/// Construct this matrix using an angle. This matrix becomes
	/// an orthonormal rotation matrix.
	explicit Mat22(float angle)
	{
		// TODO_ERIN compute sin+cos together.
		float c = cosf(angle), s = sinf(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	/// Initialize this matrix using columns.
	void Set(const Vec2& c1, const Vec2& c2)
	{
		col1 = c1;
		col2 = c2;
	}

	/// Initialize this matrix using an angle. This matrix becomes
	/// an orthonormal rotation matrix.
	void Set(float angle)
	{
		float c = cosf(angle), s = sinf(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	/// Set this to the identity matrix.
	void SetIdentity()
	{
		col1.x = 1.0f; col2.x = 0.0f;
		col1.y = 0.0f; col2.y = 1.0f;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		col1.x = 0.0f; col2.x = 0.0f;
		col1.y = 0.0f; col2.y = 0.0f;
	}

	/// Extract the angle from this matrix (assumed to be
	/// a rotation matrix).
	float GetAngle() const
	{
		return Atan2(col1.y, col1.x);
	}

	Mat22 GetInverse() const
	{
		float a = col1.x, b = col2.x, c = col1.y, d = col2.y;
		Mat22 B;
		float det = a * d - b * c;
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		B.col1.x =  det * d;	B.col2.x = -det * b;
		B.col1.y = -det * c;	B.col2.y =  det * a;
		return B;
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	Vec2 Solve(const Vec2& b) const
	{
		float a11 = col1.x, a12 = col2.x, a21 = col1.y, a22 = col2.y;
		float det = a11 * a22 - a12 * a21;
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		Vec2 x;
		x.x = det * (a22 * b.x - a12 * b.y);
		x.y = det * (a11 * b.y - a21 * b.x);
		return x;
	}

	Vec2 col1, col2;
};

/// A 3-by-3 matrix. Stored in column-major order.
struct Mat33
{
	/// The default constructor does nothing (for performance).
	Mat33() {}

	/// Construct this matrix using columns.
	Mat33(const Vec3& c1, const Vec3& c2, const Vec3& c3)
	{
		col1 = c1;
		col2 = c2;
		col3 = c3;
	}

	/// Set this matrix to all zeros.
	void SetZero()
	{
		col1.SetZero();
		col2.SetZero();
		col3.SetZero();
	}

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases.
	Vec3 Solve33(const Vec3& b) const;

	/// Solve A * x = b, where b is a column vector. This is more efficient
	/// than computing the inverse in one-shot cases. Solve only the upper
	/// 2-by-2 matrix equation.
	Vec2 Solve22(const Vec2& b) const;

	Vec3 col1, col2, col3;
};

/// A transform contains translation and rotation. It is used to represent
/// the position and orientation of rigid frames.
struct Transform
{
	/// The default constructor does nothing (for performance).
	Transform() {}

	/// Initialize using a position vector and a rotation matrix.
	Transform(const Vec2& position, const Mat22& R) : position(position), R(R) {}

	/// Set this to the identity transform.
	void SetIdentity()
	{
		position.SetZero();
		R.SetIdentity();
	}

	/// Set this based on the position and angle.
	void Set(const Vec2& p, float angle)
	{
		position = p;
		R.Set(angle);
	}

	/// Calculate the angle that the rotation matrix represents.
	float GetAngle() const
	{
		return Atan2(R.col1.y, R.col1.x);
	}

	Vec2 position;
	Mat22 R;
};

/// This describes the motion of a body/shape for TOI computation.
/// Shapes are defined with respect to the body origin, which may
/// no coincide with the center of mass. However, to support dynamics
/// we must interpolate the center of mass position.
struct Sweep
{
	/// Get the interpolated transform at a specific time.
	/// @param alpha is a factor in [0,1], where 0 indicates t0.
	void GetTransform(Transform* xf, float alpha) const;

	/// Advance the sweep forward, yielding a new initial state.
	/// @param t the new initial time.
	void Advance(float t);

	/// Normalize the angles.
	void Normalize();

	Vec2 localCenter;	///< local center of mass position
	Vec2 c0, c;		///< center world positions
	float a0, a;		///< world angles
};


extern const Vec2 Vec2_zero;
extern const Mat22 Mat22_identity;
extern const Transform Transform_identity;

/// Perform the dot product on two vectors.
inline float Dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline float Cross(const Vec2& a, const Vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a vector and a scalar. In 2D this produces
/// a vector.
inline Vec2 Cross(const Vec2& a, float s)
{
	return Vec2(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a vector. In 2D this produces
/// a vector.
inline Vec2 Cross(float s, const Vec2& a)
{
	return Vec2(-s * a.y, s * a.x);
}

/// Multiply a matrix times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another.
inline Vec2 Mul(const Mat22& A, const Vec2& v)
{
	return Vec2(A.col1.x * v.x + A.col2.x * v.y, A.col1.y * v.x + A.col2.y * v.y);
}

/// Multiply a matrix transpose times a vector. If a rotation matrix is provided,
/// then this transforms the vector from one frame to another (inverse transform).
inline Vec2 MulT(const Mat22& A, const Vec2& v)
{
	return Vec2(Dot(v, A.col1), Dot(v, A.col2));
}

/// Add two vectors component-wise.
inline Vec2 operator + (const Vec2& a, const Vec2& b)
{
	return Vec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline Vec2 operator - (const Vec2& a, const Vec2& b)
{
	return Vec2(a.x - b.x, a.y - b.y);
}

inline Vec2 operator * (float s, const Vec2& a)
{
	return Vec2(s * a.x, s * a.y);
}

inline bool operator == (const Vec2& a, const Vec2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline float Distance(const Vec2& a, const Vec2& b)
{
	Vec2 c = a - b;
	return c.Length();
}

inline float DistanceSquared(const Vec2& a, const Vec2& b)
{
	Vec2 c = a - b;
	return Dot(c, c);
}

inline Vec3 operator * (float s, const Vec3& a)
{
	return Vec3(s * a.x, s * a.y, s * a.z);
}

/// Add two vectors component-wise.
inline Vec3 operator + (const Vec3& a, const Vec3& b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/// Subtract two vectors component-wise.
inline Vec3 operator - (const Vec3& a, const Vec3& b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/// Perform the dot product on two vectors.
inline float Dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// Perform the cross product on two vectors.
inline Vec3 Cross(const Vec3& a, const Vec3& b)
{
	return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Mat22 operator + (const Mat22& A, const Mat22& B)
{
	return Mat22(A.col1 + B.col1, A.col2 + B.col2);
}

// A * B
inline Mat22 Mul(const Mat22& A, const Mat22& B)
{
	return Mat22(Mul(A, B.col1), Mul(A, B.col2));
}

// A^T * B
inline Mat22 MulT(const Mat22& A, const Mat22& B)
{
	Vec2 c1(Dot(A.col1, B.col1), Dot(A.col2, B.col1));
	Vec2 c2(Dot(A.col1, B.col2), Dot(A.col2, B.col2));
	return Mat22(c1, c2);
}

/// Multiply a matrix times a vector.
inline Vec3 Mul(const Mat33& A, const Vec3& v)
{
	return v.x * A.col1 + v.y * A.col2 + v.z * A.col3;
}

inline Vec2 Mul(const Transform& T, const Vec2& v)
{
	float x = T.position.x + T.R.col1.x * v.x + T.R.col2.x * v.y;
	float y = T.position.y + T.R.col1.y * v.x + T.R.col2.y * v.y;

	return Vec2(x, y);
}

inline Vec2 MulT(const Transform& T, const Vec2& v)
{
	return MulT(T.R, v - T.position);
}

inline Vec2 Abs(const Vec2& a)
{
	return Vec2(Abs(a.x), Abs(a.y));
}

inline Mat22 Abs(const Mat22& A)
{
	return Mat22(Abs(A.col1), Abs(A.col2));
}

template <typename T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}

inline Vec2 Min(const Vec2& a, const Vec2& b)
{
	return Vec2(Min(a.x, b.x), Min(a.y, b.y));
}

template <typename T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}

inline Vec2 Max(const Vec2& a, const Vec2& b)
{
	return Vec2(Max(a.x, b.x), Max(a.y, b.y));
}

template <typename T>
inline T Clamp(T a, T low, T high)
{
	return Max(low, Min(a, high));
}

inline Vec2 Clamp(const Vec2& a, const Vec2& low, const Vec2& high)
{
	return Max(low, Min(a, high));
}

template<typename T> inline void Swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

/// "Next Largest Power of 2
/// Given a binary integer value x, the next largest power of 2 can be computed by a SWAR algorithm
/// that recursively "folds" the upper bits into the lower bits. This process yields a bit vector with
/// the same most significant 1 as x, but all 1's below it. Adding 1 to that value yields the next
/// largest power of 2. For a 32-bit value:"
inline unsigned int NextPowerOfTwo(unsigned int x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return x + 1;
}

inline bool IsPowerOfTwo(unsigned int x)
{
	bool result = x > 0 && (x & (x - 1)) == 0;
	return result;
}

inline void Sweep::GetTransform(Transform* xf, float alpha) const
{
	xf->position = (1.0f - alpha) * c0 + alpha * c;
	float angle = (1.0f - alpha) * a0 + alpha * a;
	xf->R.Set(angle);

	// Shift to origin
	xf->position -= Mul(xf->R, localCenter);
}

inline void Sweep::Advance(float t)
{
	c0 = (1.0f - t) * c0 + t * c;
	a0 = (1.0f - t) * a0 + t * a;
}

/// Normalize an angle in radians to be between -pi and pi
inline void Sweep::Normalize()
{
	float twoPi = 2.0f * _pi;
	float d =  twoPi * floorf(a0 / twoPi);
	a0 -= d;
	a -= d;
}

#endif
