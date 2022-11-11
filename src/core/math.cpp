#include "stdafx.h"
#include "core/math.h"
#include <float.h>

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
        v3f transform(const m4f& m, const v3f& v, const f32& w)
        {
            return
            {
                m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * w,
                m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * w,
                m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * w,
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

        m4f inverse(const m4f& m)
        {
            // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
            f32 A2323 = m.m22 * m.m33 - m.m23 * m.m32;
            f32 A1323 = m.m21 * m.m33 - m.m23 * m.m31;
            f32 A1223 = m.m21 * m.m32 - m.m22 * m.m31;
            f32 A0323 = m.m20 * m.m33 - m.m23 * m.m30;
            f32 A0223 = m.m20 * m.m32 - m.m22 * m.m30;
            f32 A0123 = m.m20 * m.m31 - m.m21 * m.m30;
            f32 A2313 = m.m12 * m.m33 - m.m13 * m.m32;
            f32 A1313 = m.m11 * m.m33 - m.m13 * m.m31;
            f32 A1213 = m.m11 * m.m32 - m.m12 * m.m31;
            f32 A2312 = m.m12 * m.m23 - m.m13 * m.m22;
            f32 A1312 = m.m11 * m.m23 - m.m13 * m.m21;
            f32 A1212 = m.m11 * m.m22 - m.m12 * m.m21;
            f32 A0313 = m.m10 * m.m33 - m.m13 * m.m30;
            f32 A0213 = m.m10 * m.m32 - m.m12 * m.m30;
            f32 A0312 = m.m10 * m.m23 - m.m13 * m.m20;
            f32 A0212 = m.m10 * m.m22 - m.m12 * m.m20;
            f32 A0113 = m.m10 * m.m31 - m.m11 * m.m30;
            f32 A0112 = m.m10 * m.m21 - m.m11 * m.m20;

            f32 det = m.m00 * ( m.m11 * A2323 - m.m12 * A1323 + m.m13 * A1223 ) 
            - m.m01 * ( m.m10 * A2323 - m.m12 * A0323 + m.m13 * A0223 ) 
            + m.m02 * ( m.m10 * A1323 - m.m11 * A0323 + m.m13 * A0123 ) 
            - m.m03 * ( m.m10 * A1223 - m.m11 * A0223 + m.m12 * A0123 );
            det = 1 / det;

            m4f result = {};

            result.m00 = det *   ( m.m11 * A2323 - m.m12 * A1323 + m.m13 * A1223 );
            result.m01 = det * - ( m.m01 * A2323 - m.m02 * A1323 + m.m03 * A1223 );
            result.m02 = det *   ( m.m01 * A2313 - m.m02 * A1313 + m.m03 * A1213 );
            result.m03 = det * - ( m.m01 * A2312 - m.m02 * A1312 + m.m03 * A1212 );
            result.m10 = det * - ( m.m10 * A2323 - m.m12 * A0323 + m.m13 * A0223 );
            result.m11 = det *   ( m.m00 * A2323 - m.m02 * A0323 + m.m03 * A0223 );
            result.m12 = det * - ( m.m00 * A2313 - m.m02 * A0313 + m.m03 * A0213 );
            result.m13 = det *   ( m.m00 * A2312 - m.m02 * A0312 + m.m03 * A0212 );
            result.m20 = det *   ( m.m10 * A1323 - m.m11 * A0323 + m.m13 * A0123 );
            result.m21 = det * - ( m.m00 * A1323 - m.m01 * A0323 + m.m03 * A0123 );
            result.m22 = det *   ( m.m00 * A1313 - m.m01 * A0313 + m.m03 * A0113 );
            result.m23 = det * - ( m.m00 * A1312 - m.m01 * A0312 + m.m03 * A0112 );
            result.m30 = det * - ( m.m10 * A1223 - m.m11 * A0223 + m.m12 * A0123 );
            result.m31 = det *   ( m.m00 * A1223 - m.m01 * A0223 + m.m02 * A0123 );
            result.m32 = det * - ( m.m00 * A1213 - m.m01 * A0213 + m.m02 * A0113 );
            result.m33 = det *   ( m.m00 * A1212 - m.m01 * A0212 + m.m02 * A0112 );

            return result;
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

        f32 fwrap(f32 x, f32 min, f32 max)
        {
            if (min > max)
                return fwrap(x, max, min);
            return (x >= 0 ? min : max) + fmodf(x, max - min);
        }

        // =============================
        // Primitives
        // =============================
        Box box_union(const Box& b, const v3f& v)
        {
            Box result = b;
            result.min.x = MIN(b.min.x, v.x);
            result.min.y = MIN(b.min.y, v.y);
            result.min.z = MIN(b.min.z, v.z);
            result.max.x = MAX(b.max.x, v.x);
            result.max.y = MAX(b.max.y, v.y);
            result.max.z = MAX(b.max.z, v.z);
            return result;
        }
        
        f32 get_triangle_area(const v3f& v0, const v3f& v1, const v3f& v2)
        {
            f32 e0 = len(v1 - v0);
            f32 e1 = len(v2 - v1);
            f32 e2 = len(v0 - v2);
            f32 s = (e0 + e1 + e2) * 0.5f;
            if (fabs(s) < 0.000001f) return 0.f;
            return sqrt(s * (s - e0) * (s - e1) * (s - e2));
        }

		// =============================
		// Raycasting
		// =============================
        v3f get_barycentric_coordinates(const v3f& v, const v3f& v0, const v3f& v1, const v3f& v2)
        {
            //f32 a = get_triangle_area(v0, v1, v2);
            //f32 a0 = get_triangle_area(v, v1, v2);
            //f32 a1 = get_triangle_area(v, v2, v0);
            //f32 a2 = get_triangle_area(v, v0, v1);
            //return
            //{
            //    a0/a,
            //    a1/a,
            //    a2/a
            //};

            v3f result = {};

            v3f e0 = v1 - v0;
            v3f e1 = v2 - v0;
            v3f e2 = v - v0;
            f32 d00 = dot(e0, e0);
            f32 d01 = dot(e0, e1);
            f32 d11 = dot(e1, e1);
            f32 d20 = dot(e2, e0);
            f32 d21 = dot(e2, e1);
            f32 denom = d00 * d11 - d01 * d01;
            result.y = (d11 * d20 - d01 * d21) / denom;
            result.z = (d00 * d21 - d01 * d20) / denom;
            result.x = 1.0f - result.y - result.z;

            return result;
        }

		bool raycast_triangle(const v3f& ray_origin, const v3f& ray_dir, const v3f& v0, const v3f& v1, const v3f& v2, v3f* out)
		{
            // Moller-Trumbore ray-triangle intersection
            v3f v0v1 = v1 - v0; 
            v3f v0v2 = v2 - v0; 
            v3f pvec = cross(ray_dir, v0v2); 
            f32 det = dot(v0v1, pvec); 
            // ray and triangle are parallel if det is close to 0
            if (det < 0.0000001) return false;
            f32 invDet = 1 / det;
 
            v3f tvec = ray_origin - v0; 
            f32 u = dot(tvec, pvec) * invDet;
            if (u < 0 || u > 1) return false; 
 
            v3f qvec = cross(tvec, v0v1); 
            f32 v = dot(ray_dir, qvec) * invDet; 
            if (v < 0 || u + v > 1) return false; 
 
            f32 t = dot(v0v2, qvec) * invDet;
            if (t < 0.0000001) return false;
 
            v3f p = ray_origin + (t * ray_dir);

            if (out) *out = p;

			return true;
		}
        
		bool raycast_sphere(const v3f& ray_origin, const v3f& ray_dir, const Sphere& sphere, v3f* out)
		{
            // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
            v3f L = sphere.center - ray_origin;
            f32 t_ca = dot(L, ray_dir);
            if (t_ca < 0) return false;

            f32 d = sqrt(dot(L, L) - t_ca * t_ca);
            if (d < 0 || d > sphere.radius) return false;

            f32 t_hc = sqrt(sphere.radius * sphere.radius - d * d);
            f32 t_0 = t_ca - t_hc;
            f32 t_1 = t_ca + t_hc;      // TODO_MATH: Currently not used (second point of intersection)

            if(out) *out = ray_origin + (t_0 * ray_dir);

			return true;
		}
        
        void swap(f32& a, f32& b)
        {
            f32 t = a;
            a = b;
            b = t;
        }

		bool raycast_box(const v3f& ray_origin, const v3f& ray_dir, const Box& box, v3f* out)
		{
            f32 tmin = (box.min.x - ray_origin.x) / ray_dir.x; 
            f32 tmax = (box.max.x - ray_origin.x) / ray_dir.x; 
 
            if (tmin > tmax) swap(tmin, tmax); 
 
            f32 tymin = (box.min.y - ray_origin.y) / ray_dir.y; 
            f32 tymax = (box.max.y - ray_origin.y) / ray_dir.y; 
 
            if (tymin > tymax) swap(tymin, tymax); 
 
            if ((tmin > tymax) || (tymin > tmax)) 
                return false; 
 
            if (tymin > tmin) 
                tmin = tymin; 
 
            if (tymax < tmax) 
                tmax = tymax; 
 
            f32 tzmin = (box.min.z - ray_origin.z) / ray_dir.z; 
            f32 tzmax = (box.max.z - ray_origin.z) / ray_dir.z; 
 
            if (tzmin > tzmax) swap(tzmin, tzmax); 
 
            if ((tmin > tzmax) || (tzmin > tmax)) 
                return false; 
 
            if (tzmin > tmin) 
                tmin = tzmin; 
 
            if (tzmax < tmax) 
                tmax = tzmax; 
 
            if (out) *out = ray_origin + (tmin * ray_dir);

            return true;
		}
	}
}