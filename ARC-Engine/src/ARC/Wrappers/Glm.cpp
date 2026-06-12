#include "arc_pch.h"
#include "glm\gtx\transform.hpp"
#include "Glm.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace arc {
	mat4 convert<mat4, transform_2d>::Conv(const transform_2d& pRhs)
	{
		return glm::translate(mat4(1.0f), vec3(pRhs.Location.x, pRhs.Location.y, pRhs.Location.z)) *
			glm::rotate(mat4(1.0f), pRhs.Rotation, vec3(0, 0, 1)) *
			glm::scale(mat4(1.0f), vec3(pRhs.Scale.x, pRhs.Scale.y, 1.0f));
	}

	bool convert<transform_3d, mat4>::CanConv(const mat4& pRhs)
	{
		using namespace glm;
		using T = float;

		return !epsilonEqual(pRhs[3][3], static_cast<float>(0), epsilon<T>());
	}
	transform_3d convert<transform_3d, mat4>::Conv(const mat4& pRhs)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 localMatrix(pRhs);
		transform_3d rval;

		// Normalize the matrix.
		ARC_CORE_ASSERT(!epsilonEqual(localMatrix[3][3], static_cast<float>(0), epsilon<T>()))

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// clear the perspective partition
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
			localMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		rval.Location = vec3(localMatrix[3].x, localMatrix[3].y, localMatrix[3].z);
		localMatrix[3] = vec4(0, 0, 0, localMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row.
		rval.Scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		rval.Scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		rval.Scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		vec3 Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rval.Rotation.y = asin(-Row[0][2]);
		if (cos(rval.Rotation.y) != 0) {
			rval.Rotation.x = atan2(Row[1][2], Row[2][2]);
			rval.Rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rval.Rotation.x = atan2(-Row[2][0], Row[1][1]);
			rval.Rotation.z = 0;
		}
		return rval;
	}

	transform_2d convert<transform_2d, mat4>::Conv(const mat4& pRhs)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 localMatrix(pRhs);
		transform_2d rval;

		// Normalize the matrix.
		ARC_CORE_ASSERT(!epsilonEqual(localMatrix[3][3], static_cast<float>(0), epsilon<T>()))

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// clear the perspective partition
				localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
				localMatrix[3][3] = static_cast<T>(1);
			}

		// Next take care of translation (easy).
		rval.Location = vec3(localMatrix[3].x, localMatrix[3].y, localMatrix[3].z);
		localMatrix[3] = vec4(0, 0, 0, localMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row.
		rval.Scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		rval.Scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		vec3 Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rval.Rotation = (cos(asin(-Row[0][2])) != 0) ? atan2(Row[0][1], Row[0][0]) : 0;
		
		return rval;
	}
}