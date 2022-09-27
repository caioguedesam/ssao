#pragma once
#include "stdafx.h"

// Math definitions
#define PI 3.14159265358979323846
#define MIN(A, B) A < B ? A : B
#define MAX(A, B) A > B ? A : B
#define CLAMP(V, A, B) MAX(A, MIN(V, B))

// Typedefs
typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef float		f32;
typedef double		f64;

namespace Ty
{
	namespace Math
	{
		// =============================
		// Trigonometry
		// =============================
		f32 to_deg(const f32& rad);
		f32 to_rad(const f32& deg);

		// =============================
		// Vector2
		// =============================
		struct v2f
		{
			union
			{
				f32 x;
				f32 u;
			};
			union
			{
				f32 y;
				f32 v;
			};
		};

		v2f operator+(const v2f& lhs, const v2f& rhs);
		v2f operator-(const v2f& lhs, const v2f& rhs);
		v2f operator*(const f32& a, const v2f& v);
		v2f operator*(const v2f& v, const f32& a);
		v2f operator/(const v2f& v, const f32& a);
		bool operator==(const v2f& lhs, const v2f& rhs);
		f32 dot(const v2f& lhs, const v2f& rhs);
		f32 cross(const v2f& lhs, const v2f& rhs);
		f32 sqrlen(const v2f& v);
		f32 len(const v2f& v);
		v2f normalize(const v2f& v);
		f32 angle(const v2f& lhs, const v2f& rhs);

		struct v2u
		{
			u32 x;
			u32 y;
		};
		v2u operator+(const v2u& lhs, const v2u& rhs);
		v2u operator-(const v2u& lhs, const v2u& rhs);
		bool operator==(const v2u& lhs, const v2u& rhs);

		struct v2i
		{
			i32 x;
			i32 y;
		};
		v2i operator+(const v2i& lhs, const v2i& rhs);
		v2i operator-(const v2i& lhs, const v2i& rhs);
		bool operator==(const v2i& lhs, const v2i& rhs);

		// =============================
		// Vector3
		// =============================
		struct v3f
		{
			union
			{
				f32 x;
				f32 r;
			};
			union
			{
				f32 y;
				f32 g;
			};
			union
			{
				f32 z;
				f32 b;
			};

			inline f32& operator[](const i32& i)
			{
				return *(&this->x + sizeof(f32) * i);
			}
		};

		v3f operator+(const v3f& lhs, const v3f& rhs);
		v3f operator-(const v3f& lhs, const v3f& rhs);
		v3f operator*(const f32& a, const v3f& v);
		v3f operator*(const v3f& v, const f32& a);
		v3f operator/(const v3f& v, const f32& a);
		bool operator==(const v3f& lhs, const v3f& rhs);
		f32 dot(const v3f& lhs, const v3f& rhs);
		v3f cross(const v3f& lhs, const v3f& rhs);
		f32 sqrlen(const v3f& v);
		f32 len(const v3f& v);
		v3f normalize(const v3f& v);

		// =============================
		// Matrix4x4
		// =============================
		struct m4f
		{
			f32 m00; f32 m01; f32 m02; f32 m03;
			f32 m10; f32 m11; f32 m12; f32 m13;
			f32 m20; f32 m21; f32 m22; f32 m23;
			f32 m30; f32 m31; f32 m32; f32 m33;
		};

		m4f operator+(const m4f& lhs, const m4f& rhs);
		m4f operator-(const m4f& lhs, const m4f& rhs);
		bool operator==(const m4f& lhs, const m4f& rhs);
		m4f operator*(const f32& a, const m4f& m);
		m4f operator*(const m4f& lhs, const m4f& rhs);
		m4f transpose(const m4f& m);

		// TODO_MATH: Implement more useful matrix operations
		// inverse, determinant, elementary operations, etc.
		// this might require m3f and m2f, and [] operator.

		m4f identity();
		m4f scale(const f32& scale_x, const f32& scale_y, const f32& scale_z);
		m4f rotation(const f32& angle, const f32& axis_x, const f32& axis_y, const f32& axis_z);
		m4f translation(const f32& move_x, const f32& move_y, const f32& move_z);

		m4f look_at(const v3f& center, const v3f& target, const v3f up);
		m4f perspective(const f32& fov_y, const f32& aspect, const f32& near_plane, const f32& far_plane);

		// =============================
		// Utilities
		// =============================
		inline f32 lerp(const f32& a, const f32& b, const f32& t)
		{
			return a + (b - a) * CLAMP(t, 0, 1);
		}

		// TODO_MATH: Deprecate this to use new math library
		namespace Primitives
		{
			struct u32_rect
			{
				uint32_t w = 0.f;
				uint32_t h = 0.f;
				uint32_t x = 0.f;
				uint32_t y = 0.f;
			};
		}
	}

}