#include "stdafx.h"
#include "core/math.h"

namespace Ty
{
	namespace Math
	{
		// =============================
		// Trigonometry
		// =============================
		f32 to_deg(const f32& rad)
		{
			return rad * 180.f / PI;
		}
		f32 to_rad(const f32& deg)
		{
			return deg * PI / 180.f;
		}

		// =============================
		// Vector2
		// =============================
		v2f operator+(const v2f& lhs, const v2f& rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y };
		}
		v2f operator-(const v2f& lhs, const v2f& rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y };
		}
		v2f operator*(const f32& a, const v2f& v)
		{
			return { a * v.x, a * v.y };
		}
		v2f operator*(const v2f& v, const f32& a)
		{
			return a * v;
		}
		v2f operator/(const v2f& v, const f32& a)
		{
			return { v.x / a, v.y / a };
		}
		bool operator==(const v2f& lhs, const v2f& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
		f32 dot(const v2f& lhs, const v2f& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		f32 cross(const v2f& lhs, const v2f& rhs)
		{
			// Since cross is only defined in 3D, this returns only Z coordinate, 
			// which can be used to calculate winding order between points, for example.
			return { lhs.x * rhs.y - lhs.y * rhs.x };
		}
		f32 sqrlen(const v2f& v)
		{
			return dot(v, v);
		}
		f32 len(const v2f& v)
		{
			return sqrt(sqrlen(v));
		}
		v2f normalize(const v2f& v)
		{
			return v / len(v);
		}
		f32 angle(const v2f& lhs, const v2f& rhs)		// Angle in radians between 2 vectors, from 0 to PI
		{
			return acos(dot(normalize(lhs), normalize(rhs)));
		}


		v2u operator+(const v2u& lhs, const v2u& rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y };
		}
		v2u operator-(const v2u& lhs, const v2u& rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y };
		}
		bool operator==(const v2u& lhs, const v2u& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}

		v2i operator+(const v2i& lhs, const v2i& rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y };
		}
		v2i operator-(const v2i& lhs, const v2i& rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y };
		}
		bool operator==(const v2i& lhs, const v2i& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}

		// =============================
		// Vector3
		// =============================
		v3f operator+(const v3f& lhs, const v3f& rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
		}
		v3f operator-(const v3f& lhs, const v3f& rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
		}
		v3f operator*(const f32& a, const v3f& v)
		{
			return { a * v.x, a * v.y, a * v.z };
		}
		v3f operator*(const v3f& v, const f32& a)
		{
			return a * v;
		}
		v3f operator/(const v3f& v, const f32& a)
		{
			return { v.x / a, v.y / a, v.z / a };
		}
		bool operator==(const v3f& lhs, const v3f& rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}
		f32 dot(const v3f& lhs, const v3f& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		v3f cross(const v3f& lhs, const v3f& rhs)
		{
			return
			{
				lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x
			};
		}
		f32 sqrlen(const v3f& v)
		{
			return dot(v, v);
		}
		f32 len(const v3f& v)
		{
			return sqrt(sqrlen(v));
		}
		v3f normalize(const v3f& v)
		{
			return v / len(v);
		}

		// =============================
		// Matrix4x4
		// =============================
		m4f operator+(const m4f& lhs, const m4f& rhs)
		{
			return
			{
				lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02, lhs.m03 + rhs.m03,
				lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13,
				lhs.m20 + rhs.m20, lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23,
				lhs.m30 + rhs.m30, lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33
			};
		}
		m4f operator-(const m4f& lhs, const m4f& rhs)
		{
			return
			{
				lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02, lhs.m03 - rhs.m03,
				lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13,
				lhs.m20 - rhs.m20, lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23,
				lhs.m30 - rhs.m30, lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33
			};
		}
		bool operator==(const m4f& lhs, const m4f& rhs)
		{
			return
				lhs.m00 == rhs.m00 && lhs.m01 == rhs.m01 && lhs.m02 == rhs.m02 && lhs.m03 == rhs.m03 &&
				lhs.m10 == rhs.m10 && lhs.m11 == rhs.m11 && lhs.m12 == rhs.m12 && lhs.m13 == rhs.m13 &&
				lhs.m20 == rhs.m20 && lhs.m21 == rhs.m21 && lhs.m22 == rhs.m22 && lhs.m23 == rhs.m23 &&
				lhs.m30 == rhs.m30 && lhs.m31 == rhs.m31 && lhs.m32 == rhs.m32 && lhs.m33 == rhs.m33;
		}
		m4f operator*(const f32& a, const m4f& m)
		{
			return
			{
				a * m.m00, a * m.m01, a * m.m02, a * m.m03,
				a * m.m10, a * m.m11, a * m.m12, a * m.m13,
				a * m.m20, a * m.m21, a * m.m22, a * m.m23,
				a * m.m30, a * m.m31, a * m.m32, a * m.m33
			};
		}
		m4f operator*(const m4f& lhs, const m4f& rhs)
		{
			return
			{
				// Row 0
				lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30,
				lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31,
				lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32,
				lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33,

				// Row 1
				lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30,
				lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31,
				lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32,
				lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33,

				// Row 2
				lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30,
				lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31,
				lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32,
				lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33,

				// Row 3
				lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30,
				lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31,
				lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32,
				lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33,
			};
		}
		m4f transpose(const m4f& m)
		{
			return
			{
				m.m00, m.m10, m.m20, m.m30,
				m.m01, m.m11, m.m21, m.m31,
				m.m02, m.m12, m.m22, m.m32,
				m.m03, m.m13, m.m23, m.m33,
			};
		}

		m4f identity()
		{
			return
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1,
			};
		}
		m4f scale(const f32& scale_x, const f32& scale_y, const f32& scale_z)
		{
			return
			{
				scale_x,	0,			0,			0,
				0,			scale_y,	0,			0,
				0,			0,			scale_z,	0,
				0,			0,			0,			1
			};
		}
		m4f rotation(const f32& angle, const f32& axis_x, const f32& axis_y, const f32& axis_z)
		{
			f32 ang_sin = sin(angle);
			f32 ang_cos = cos(angle);
			f32 inv_cos = 1 - ang_cos;
			return
			{
				axis_x * axis_x * inv_cos + ang_cos,			axis_y * axis_x * inv_cos - axis_z * ang_sin,	axis_z * axis_x * inv_cos + axis_y * ang_sin,	0,
				axis_x * axis_y * inv_cos + axis_z * ang_sin,	axis_y * axis_y * inv_cos + ang_cos,			axis_z * axis_y * inv_cos - axis_x * ang_sin,	0,
				axis_x * axis_z * inv_cos - axis_y * ang_sin,	axis_y * axis_z * inv_cos + axis_x * ang_sin,	axis_z * axis_z * inv_cos + ang_cos,			0,
				0, 0, 0, 1,
			};
		}
		m4f translation(const f32& move_x, const f32& move_y, const f32& move_z)
		{
			return
			{
				1, 0, 0, move_x,
				0, 1, 0, move_y,
				0, 0, 1, move_z,
				0, 0, 0, 1
			};
		}

		m4f look_at(const v3f& center, const v3f& target, const v3f up)
		{
			//v3f look_dir	= normalize(target - center);
			v3f look_dir	= normalize(center - target);		// TODO_MATH: OpenGL convention (camera points to z negative)
			v3f look_right	= normalize(cross(up, look_dir));
			v3f look_up		= cross(look_dir, look_right);
			m4f look_rotation =
			{
				look_right.x, look_right.y, look_right.z, 0,
				look_up.x, look_up.y, look_up.z, 0,
				look_dir.x, look_dir.y, look_dir.z, 0,
				0, 0, 0, 1
			};
			m4f look_translation =
			{
				1, 0, 0, -center.x,
				0, 1, 0, -center.y,
				0, 0, 1, -center.z,
				0, 0, 0, 1
			};
			return look_rotation * look_translation;
		}
		m4f perspective(const f32& fov_y, const f32& aspect, const f32& near_plane, const f32& far_plane)
		{
			f32 top = tan(fov_y / 2) * near_plane;
			f32 bottom = -top;
			f32 right = top * aspect;
			f32 left = bottom * aspect;
			m4f res =
			{
				(2 * near_plane) / (right - left), 0, 0, 0,
				0, (2 * near_plane) / (top - bottom), 0, 0,
				(right + left) / (right - left), (top + bottom) / (top - bottom), -(far_plane + near_plane) / (far_plane - near_plane), -1,
				0, 0, -(2 * far_plane * near_plane) / (far_plane - near_plane), 0
			};
			return transpose(res);		// TODO_MATH: OpenGL convention (matrix layout as column major, figure this out later?)
		}

		// =============================
		// Utilities
		// =============================
		f32 lerp(const f32& a, const f32& b, const f32& t)
		{
			return a + (b - a) * CLAMP(t, 0, 1);
		}

		v2f lerp(const v2f& a, const v2f& b, const f32& t)
		{
			return
			{
				lerp(a.x, b.x, t),
				lerp(a.y, b.y, t),
			};
		}

		v3f lerp(const v3f& a, const v3f& b, const f32& t)
		{
			return
			{
				lerp(a.x, b.x, t),
				lerp(a.y, b.y, t),
				lerp(a.z, b.z, t),
			};
		}
	}
}