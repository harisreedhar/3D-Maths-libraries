#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector3f.h"
#include "Vector2f.h"
#include "Plane.h"
#include "Quaternion.h"
#include <string>

class Matrix4x4
{
public:
	float m[4][4];

	//	identity matrix
	Matrix4x4()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = 1.0f;
		m[2][3] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	~Matrix4x4() = default;

	static Matrix4x4 Translate(Vector3f translation)
	{
		Matrix4x4 result;
		result.m[0][3] = translation.x;
		result.m[1][3] = translation.y;
		result.m[2][3] = translation.z;
		result.m[0][0] = 1.0f;
		result.m[1][1] = 1.0f;
		result.m[2][2] = 1.0f;
		result.m[3][3] = 1.0f;
		return result;
	}

	static Matrix4x4 Scale(Vector3f scale)
	{
		Matrix4x4 result;
		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		result.m[2][2] = scale.z;
		result.m[3][3] = 1.0f;
		return result;
	}

	static Matrix4x4 RotateX(float angle)
	{
		Matrix4x4 result;
		result.m[1][1] = cos(angle);
		result.m[1][2] = -sin(angle);
		result.m[2][1] = sin(angle);
		result.m[2][2] = cos(angle);
		return result;
	}

	static Matrix4x4 RotateY(float angle)
	{
		Matrix4x4 result;
		result.m[0][0] = cos(angle);
		result.m[2][0] = sin(angle);
		result.m[2][0] = -sin(angle);
		result.m[2][2] = cos(angle);
		return result;
	}

	static Matrix4x4 RotateZ(float angle)
	{
		Matrix4x4 result;
		result.m[0][0] = cos(angle);
		result.m[0][1] = -sin(angle);
		result.m[1][0] = sin(angle);
		result.m[1][1] = cos(angle);
		return result;
	}

	static Matrix4x4 Rotate(float r, float i, float j, float k)
	{
		Matrix4x4 result(
		1 - 2 * j * j - 2 * k * k,
		2 * i * j - 2 * k * r,
		2 * i * k + 2 * j * r,
		0.0f,

		2 * i * j + 2 * r * k,
		1 - 2 * i * i - 2 * k * k,
		2 * j * k - 2 * r * i,
		0.0f,

		2 * i * k - 2 * r * j,
		2 * j * k + 2 * r * i,
		1 - 2 * i * i - 2 * j * j,
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f);
		return result;
	}

	static Matrix4x4 Rotate(Quaternion rotation)
	{
		return Rotate(rotation.r, rotation.i, rotation.j, rotation.k);
	}

	// Right Hand Perspective Matrix
	static Matrix4x4 PerspectiveRH(float fovY, float aspectRatio, float nearDepth, float farDepth)
	{
		float cosFov = cos(fovY * 0.5f);
		float sinFov = sin(fovY * 0.5f);

		float height = cosFov / sinFov;
		float width = height / aspectRatio;
		float fRange = farDepth / (nearDepth - farDepth);

		Matrix4x4 result(
			width,
			0.0f,
			0.0f,
			0.0f,

			0.0f,
			height,
			0.0f,
			0.0f,

			0.0f,
			0.0f,
			fRange,
			-1.0f,

			0.0f,
			0.0f,
			fRange * nearDepth,
			0.0f);
		return result;
	}

	// Left Hand Perspective Matrix
	static Matrix4x4 PerspectiveLH(float fovY, float aspectRatio, float nearDepth, float farDepth)
	{
		float sinFov = sin(fovY * 0.5f);
		float cosFov = cos(fovY * 0.5f);

		float height = cosFov / sinFov;
		float width = height / aspectRatio;
		float fRange = farDepth / (farDepth - nearDepth);

		Matrix4x4 result(
			width,
			0.0f,
			0.0f,
			0.0f,

			0.0f,
			height,
			0.0f,
			0.0f,

			0.0f,
			0.0f,
			fRange,
			1.0f,

			0.0f,
			0.0f,
			-fRange * nearDepth,
			0.0f);
		return result;
	}

	static Matrix4x4 OrthographicRH(float left, float right, float bottom, float top, float nearDepth, float farDepth)
	{
		Matrix4x4 result(
			2 / (right - left),
			0.0f,
			0.0f,
			0.0f,

			0.0f,
			2 / (top - bottom),
			0.0f,
			0.0f,

			0.0f,
			0.0f,
			-2 / (farDepth - nearDepth),
			0.0f,

			-((right + left) / (right - left)),
			-((top + bottom) / (top - bottom)),
			-((farDepth + nearDepth) / (farDepth - nearDepth)),
			1.0f);
		return result;
	}

	static Matrix4x4 OrthographicLH(float left, float right, float bottom, float top, float nearDepth, float farDepth)
	{
		Matrix4x4 result(
			2 / (right - left),
			0.0f,
			0.0f,
			0.0f,

			0.0f,
			2 / (top - bottom),
			0.0f,
			0.0f,

			0.0f,
			0.0f,
			-2 / (farDepth - nearDepth),
			0.0f,

			-((right + left) / (right - left)),
			-((top + bottom) / (top - bottom)),
			-((farDepth + nearDepth) / (farDepth - nearDepth)),
			1.0f);

		return result;
	}

	static Matrix4x4 LookAt(Vector3f eyePosition, Vector3f lookAtPosition, Vector3f up)
	{
		Vector3f f = (lookAtPosition - eyePosition).GetNormalized();
		Vector3f s = (Vector3f::Cross(f, up)).GetNormalized();
		Vector3f u = Vector3f::Cross(s, f);

		Matrix4x4 result(
		s.x,
		u.x,
		-f.x,
		0.0f,

		s.y,
		u.y,
		-f.y,
		0.0f,

		s.z,
		u.z,
		-f.z,
		0.0f,

		-Vector3f::Dot(s, eyePosition),
		-Vector3f::Dot(up, eyePosition),
		Vector3f::Dot(f, eyePosition),
		1.0f);

		return result;
	}

	static float Determinant(const Matrix4x4& matrix)
	{
		return
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3];
	}

	static Matrix4x4 Inverse(const Matrix4x4& matrix)
	{
		Matrix4x4 result(
			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3],
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2] + matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3],
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3],
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3],
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3],
			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3],
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3],
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3],
			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3],
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3],
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3],
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3],
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2],
			matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2],
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2],
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] + matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
		);

		return MulFloat(result, Matrix4x4::Determinant(matrix));
	}

	static void FrustumPlanes(Plane planes[6], Matrix4x4 matrix)
	{
		//left
		planes[0].a = matrix(0, 3) + matrix(0, 0);
		planes[0].b = matrix.m[1][3] + matrix.m[1][0];
		planes[0].c = matrix.m[2][3] + matrix.m[2][0];
		planes[0].d = matrix.m[3][3] + matrix.m[3][0];

		//right
		planes[1].a = matrix.m[0][3] - matrix.m[0][0];
		planes[1].b = matrix.m[1][3] - matrix.m[1][0];
		planes[1].c = matrix.m[2][3] - matrix.m[2][0];
		planes[1].d = matrix.m[3][3] - matrix.m[3][0];

		//Bottom
		planes[2].a = matrix.m[0][3] + matrix.m[0][1];
		planes[2].b = matrix.m[1][3] + matrix.m[1][1];
		planes[2].c = matrix.m[2][3] + matrix.m[2][1];
		planes[2].d = matrix.m[3][3] + matrix.m[3][1];

		//Top
		planes[3].a = matrix.m[0][3] - matrix.m[0][1];
		planes[3].b = matrix.m[1][3] - matrix.m[1][1];
		planes[3].c = matrix.m[2][3] - matrix.m[2][1];
		planes[3].d = matrix.m[3][3] - matrix.m[3][1];

		//Near
		planes[4].a = matrix.m[0][2];
		planes[4].b = matrix.m[1][2];
		planes[4].c = matrix.m[2][2];
		planes[4].d = matrix.m[3][2];

		//Far
		planes[5].a = matrix.m[0][3] - matrix.m[0][2];
		planes[5].b = matrix.m[1][3] - matrix.m[1][2];
		planes[5].c = matrix.m[2][3] - matrix.m[2][2];
		planes[5].d = matrix.m[3][3] - matrix.m[3][2];

		for (int i = 0; i < 6; ++i)
		{
			planes[i].Normalize();
		}
	}

	static Matrix4x4 MulFloat(Matrix4x4 m, float scale)
	{
		return Matrix4x4(
			m.m[0][0] * scale,
			m.m[0][1] * scale,
			m.m[0][2] * scale,
			m.m[0][3] * scale,
			m.m[1][0] * scale,
			m.m[1][1] * scale,
			m.m[1][2] * scale,
			m.m[1][3] * scale,
			m.m[2][0] * scale,
			m.m[2][1] * scale,
			m.m[2][2] * scale,
			m.m[2][3] * scale,
			m.m[3][0] * scale,
			m.m[3][1] * scale,
			m.m[3][2] * scale,
			m.m[3][3] * scale
		);
	}

	static Vector2f MulVec2(Matrix4x4 matrix, Vector2f vector)
	{
		Vector2f result;
		result.x = (matrix(0, 0) * vector.x) + (matrix(0, 1) * vector.y) + (matrix(0, 3));
		result.y = (matrix(1, 0) * vector.x) + (matrix(1, 1) * vector.y) + (matrix(1, 3));
		return result;
	}

	static Vector3f MulVec3(Matrix4x4 matrix, Vector3f vector)
	{
		Vector3f result(
		(matrix(0, 0) * vector.x) + (matrix(0, 1) * vector.y) + (matrix(0, 2) * vector.z) + (matrix(0, 3)),
		(matrix(1, 0) * vector.x) + (matrix(1, 1) * vector.y) + (matrix(1, 2) * vector.z) + (matrix(1, 3)),
		(matrix(2, 0) * vector.x) + (matrix(2, 1) * vector.y) + (matrix(2, 2) * vector.z) + (matrix(2, 3)));
		return result;
	}

	Vector2f ToVector2f() const
	{
		Vector2f result(
		m[0][0] + m[0][1] + m[0][3],
		m[1][0] + m[1][1] + m[1][3]);
		return result;
	}

	Vector3f ToVector3f() const
	{
		Vector3f result(
		m[0][0] + m[0][1] + m[0][2] + m[0][3],
		m[1][0] + m[1][1] + m[1][2] + m[1][3],
		m[2][0] + m[2][1] + m[2][2] + m[2][3]);
		return Vector3f();
	}

	Vector3f ExtractTranslation() const
	{
		return Vector3f(m[0][3], m[1][3], m[2][3]);
	}

	Vector3f ExtractScale()	const
	{
		Vector3f result(
		m[0][0] + m[0][1] + m[0][2],
		m[1][0] + m[1][1] + m[1][2],
		m[2][0] + m[2][1] + m[2][2]);
		return result;
	}

	Quaternion ExtractRotation() const
	{
		Quaternion q;

		float trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > 0)
		{
			float s = 0.5f / sqrt(trace + 1.0f);
			q.r = 0.25f / s;
			q.i = (m[2][1] - m[1][2]) * s;
			q.j = (m[0][2] - m[2][0]) * s;
			q.k = (m[1][0] - m[0][1]) * s;
		}
		else
		{
			if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
				float s = 2.0f * sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
				q.r = (m[2][1] - m[1][2]) / s;
				q.i = 0.25f * s;
				q.j = (m[0][1] + m[1][0]) / s;
				q.k = (m[0][2] + m[2][0]) / s;
			}
			else if (m[1][1] > m[2][2]) {
				float s = 2.0f * sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
				q.r = (m[0][2] - m[2][0]) / s;
				q.i = (m[0][1] + m[1][0]) / s;
				q.j = 0.25f * s;
				q.k = (m[1][2] + m[2][1]) / s;
			}
			else {
				float s = 2.0f * sqrtf(1.0f + m[2][2] - m[0][0] - m[1][1]);
				q.r = (m[1][0] - m[0][1]) / s;
				q.i = (m[0][2] + m[2][0]) / s;
				q.j = (m[1][2] + m[2][1]) / s;
				q.k = 0.25f * s;
			}
		}

		return q;
	}

	float ExtractRotationX() const;
	float ExtractRotationY() const;
	float ExtractRotationZ() const;

	std::string to_string()const
	{
		std::string result;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{

				result += std::to_string(m[i][j]);
				if (j != 3)
				{
					result += ", ";
				}

			}
			if (i != 3)
			{
				result += "\n";
			}
		}

		return result;
	}

	Matrix4x4 operator*(Matrix4x4 other)
	{
		Matrix4x4 result;
		// Cache the invariants in registers
		float x = m[0][0];
		float y = m[0][1];
		float z = m[0][2];
		float w = m[0][3];
		// Perform the operation on the first row
		result.m[0][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
		result.m[0][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
		result.m[0][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
		result.m[0][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
		// Repeat for all the other rows
		x = m[1][0];
		y = m[1][1];
		z = m[1][2];
		w = m[1][3];
		result.m[1][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
		result.m[1][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
		result.m[1][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
		result.m[1][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
		x = m[2][0];
		y = m[2][1];
		z = m[2][2];
		w = m[2][3];
		result.m[2][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
		result.m[2][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
		result.m[2][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
		result.m[2][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
		x = m[3][0];
		y = m[3][1];
		z = m[3][2];
		w = m[3][3];
		result.m[3][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
		result.m[3][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
		result.m[3][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
		result.m[3][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
		return result;
	}

	float operator()(size_t row, size_t column)
	{
		return m[row][column];
	}
};

inline Vector2f operator*(const Matrix4x4& m, const Vector2f& v)
{
	return Matrix4x4::MulVec2(m, v);
}
inline Vector3f operator*(const Matrix4x4& m, const Vector3f& v)
{
	return Matrix4x4::MulVec3(m, v);
}
inline Matrix4x4 operator*(const Matrix4x4& m, const float& f)
{
	return Matrix4x4::MulFloat(m, f);
}
inline std::ostream& operator<<(std::ostream& os, Matrix4x4& m)
{
	return os << m.to_string();
}

#endif // !_MATRIX_H