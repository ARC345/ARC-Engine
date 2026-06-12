#include "arc_pch.h"
#include "PhysicsHandler.h"

namespace arc {
	void physyics_handler::ProjectVertices(std::vector<vec3>& pA, vec3 pAxis, OUT float& pMin, OUT float& pMax) {
		pMin = std::numeric_limits<float>::max();
		pMax = -std::numeric_limits<float>::max();

		for (auto _ : pA)
		{
			auto proj = math::Dot(_, pAxis);

			pMin = math::Min(pMin, proj);
			pMax = math::Max(pMax, proj);
		}
	};

	void physyics_handler::ProjectSphere(vec3 pLoc, float pRadius, vec3 pAxis, OUT float& pMin, OUT float& pMax)
	{
		auto p1 = pLoc + pAxis.Normalize()*pRadius;
		auto p2 = pLoc - pAxis.Normalize()*pRadius;

		auto d1 = math::Dot(p1, pAxis);
		auto d2 = math::Dot(p2, pAxis);

		pMin = math::Min(d1, d2);
		pMax = math::Max(d1, d2);
	}

	bool physyics_handler::is_colliding_sphere(vec3 pLoc1, vec3 pLoc2, float pRadiusSum, OUT vec3& pNormal, OUT float& pDepth)
	{
		float dist = math::DistSqr(pLoc1, pLoc2);

		if (dist > math::Sqr(pRadiusSum)) return false;

		dist = math::Sqrt(dist);
		pDepth = pRadiusSum-dist;
		pNormal  = (pLoc2 - pLoc1) / dist;
		return true;
	}

	bool physyics_handler::is_colliding_sphere(vec3 pLoc1, vec3 pLoc2, float pRadiusSum)
	{
		return math::DistSqr(pLoc1, pLoc2) <= math::Sqr(pRadiusSum);
	}


	bool physyics_handler::is_colliding_polygon2_d(std::vector<vec3>& pA, std::vector<vec3>& pB, OUT vec3& pNormal, OUT float& pDepth)
	{
		pNormal = vec3::ZeroVector();
		pDepth = std::numeric_limits<float>::max();

		auto fn = [&](std::vector<vec3>& p1) -> bool {
			for (size_t i = 0; i < p1.size(); i++)
			{
				vec3 vi = p1[i];
				vec3 vi2 = p1[(i + 1) % p1.size()];

				vec3 edge = vi2 - vi;
				vec3 axis = vec3(-edge.y, edge.x, 0);

				float min, max, min2, max2;
				ProjectVertices(pA, axis, min, max);
				ProjectVertices(pB, axis, min2, max2);

				if (min >= max2 || min2 >= max)
					return false;

				float axisDepth = math::Min(max2 - min, max - min2);
				if (axisDepth < pDepth)
				{
					pDepth = axisDepth;
					pNormal = axis;
				}
			}
			return true;
		};

		if (!fn(pA)) return false;
		if (!fn(pB)) return false;

		pDepth /= pNormal.Length();
		pNormal = pNormal.Normalize();

		if (math::Dot(	math::Mean(pB)	- math::Mean(pA),	pNormal ) < 0.f )
		{
			pNormal=pNormal*-1.f;
		}

		return true;
	}

	bool physyics_handler::is_colliding_polygon2_d_vs_sphere(std::vector<vec3>& pA, vec3 pSphereLoc, float pRadius, OUT vec3& pNormal, OUT float& pDepth)
	{
		pNormal = vec3::ZeroVector();
		pDepth = std::numeric_limits<float>::max();

		auto fn = [&](vec3 edge) -> bool {
			vec3 axis = vec3(-edge.y, edge.x, 0);

			float min, max, min2, max2;
			ProjectVertices(pA, axis, min, max);
			ProjectSphere(pSphereLoc, pRadius, axis, min2, max2);

			if (min >= max2 || min2 >= max)
				return false;

			float axisDepth = math::Min(max2 - min, max - min2);
			if (axisDepth < pDepth)
			{
				pDepth = axisDepth;
				pNormal = axis;
			}
			return true;
		};

		for (size_t i = 0; i < pA.size(); i++)
			if (!fn(pA[(i+1)%pA.size()] - pA[i]))
				return false;
		
		{
			// closest point in poly
			int ClossestPointIndex = -1;
			float MinDistSqr = std::numeric_limits<float>::max();
			for (int i = 0; i < pA.size(); i++)
			{
				auto distSqr = math::DistSqr(pA[i], pSphereLoc);
				if (MinDistSqr > distSqr)
				{
					ClossestPointIndex = i;
					MinDistSqr = distSqr;
				}
			}
			if(!fn(pA[ClossestPointIndex] - pSphereLoc)) return false;
		}

		pDepth /= pNormal.Length();
		pNormal = pNormal.Normalize();

		if (math::Dot(pSphereLoc - math::Mean(pA), pNormal) < 0.f)
		{
			pNormal = pNormal * -1.f;
		}

		return true;
	}

	void physyics_handler::ClearOverlap(vec3& pLHS, vec3& pRHS, vec3 pNormal, float pDepth)
	{
		pLHS -= pNormal * pDepth / 2.f;
		pRHS += pNormal * pDepth / 2.f;
	}

	void physyics_handler::HandleLinearCollisionResponse(vec3& pVelocity1, vec3& pVelocity2, float pMass1, float pMass2, vec3 pNormal, float pRestitution /*= 1.f*/)
	{
		float j = -(1.f + pRestitution) * math::Dot((pVelocity2 - pVelocity1), pNormal) / (1 / pMass1 + 1 / pMass2);

		pVelocity1 -= pNormal * j / pMass1;
		pVelocity2 += pNormal * j / pMass2;
	}

	void physyics_handler::HandleAngularCollisionResponse(vec3& pAngularVelocity1, vec3& pAngularVelocity2, float pMass1, float pMass2, vec3 pNormal, float pRestitution /*= 1.f*/)
	{

	}

	void physyics_handler::UpdateFrameLinearVelocity(transform_2d& pTrans, const vec3& pVelocity, float pDeltaTime)
	{
		pTrans.Location += pVelocity * pDeltaTime;
	}

	void physyics_handler::UpdateFrameAngularVelocity(transform_2d& pTrans, const vec3& pAngularVelocity, float pDeltaTime)
	{
		pTrans.Rotation += pAngularVelocity.x * pDeltaTime;
	}

	void physyics_handler::UpdateFrameAcceleration(vec3& pVelocity, const vec3& pAcceleration, float pDeltaTime)
	{
		pVelocity += pAcceleration * pDeltaTime;
	}

	void physyics_handler::UpdateFrameAngularAcceleration(vec3& pAngularVelocity, const vec3& pAngularAcceleration, float pDeltaTime)
	{
		pAngularVelocity += pAngularAcceleration * pDeltaTime;
	}

}