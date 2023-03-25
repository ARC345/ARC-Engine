#include "arc_pch.h"
#include "PhysicsHandler.h"

namespace ARC {
	void SPhysyicsHandler::ProjectVertices(std::vector<FVec3>& pA, FVec3 pAxis, OUT float& pMin, OUT float& pMax) {
		pMin = std::numeric_limits<float>::max();
		pMax = -std::numeric_limits<float>::max();

		for (auto _ : pA)
		{
			auto proj = SMath::Dot(_, pAxis);

			pMin = SMath::Min(pMin, proj);
			pMax = SMath::Max(pMax, proj);
		}
	};

	void SPhysyicsHandler::ProjectSphere(FVec3 pLoc, float pRadius, FVec3 pAxis, OUT float& pMin, OUT float& pMax)
	{
		auto p1 = pLoc + pAxis.Normalize()*pRadius;
		auto p2 = pLoc - pAxis.Normalize()*pRadius;

		auto d1 = SMath::Dot(p1, pAxis);
		auto d2 = SMath::Dot(p2, pAxis);

		pMin = SMath::Min(d1, d2);
		pMax = SMath::Max(d1, d2);
	}

	bool SPhysyicsHandler::IsCollidingSphere(FVec3 pLoc1, FVec3 pLoc2, float pRadiusSum, OUT FVec3& pNormal, OUT float& pDepth)
	{
		float dist = SMath::DistSqr(pLoc1, pLoc2);

		if (dist > SMath::Sqr(pRadiusSum)) return false;

		dist = SMath::Sqrt(dist);
		pDepth = pRadiusSum-dist;
		pNormal  = (pLoc2 - pLoc1) / dist;
		return true;
	}

	bool SPhysyicsHandler::IsCollidingSphere(FVec3 pLoc1, FVec3 pLoc2, float pRadiusSum)
	{
		return SMath::DistSqr(pLoc1, pLoc2) <= SMath::Sqr(pRadiusSum);
	}


	bool SPhysyicsHandler::IsCollidingPolygon2D(std::vector<FVec3>& pA, std::vector<FVec3>& pB, OUT FVec3& pNormal, OUT float& pDepth)
	{
		pNormal = FVec3::ZeroVector();
		pDepth = std::numeric_limits<float>::max();

		auto fn = [&](std::vector<FVec3>& p1) -> bool {
			for (size_t i = 0; i < p1.size(); i++)
			{
				FVec3 vi = p1[i];
				FVec3 vi2 = p1[(i + 1) % p1.size()];

				FVec3 edge = vi2 - vi;
				FVec3 axis = FVec3(-edge.y, edge.x, 0);

				float min, max, min2, max2;
				ProjectVertices(pA, axis, min, max);
				ProjectVertices(pB, axis, min2, max2);

				if (min >= max2 || min2 >= max)
					return false;

				float axisDepth = SMath::Min(max2 - min, max - min2);
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

		if (SMath::Dot(	SMath::Mean(pB)	- SMath::Mean(pA),	pNormal ) < 0.f )
		{
			pNormal=pNormal*-1.f;
		}

		return true;
	}

	bool SPhysyicsHandler::IsCollidingPolygon2DVsSphere(std::vector<FVec3>& pA, FVec3 pSphereLoc, float pRadius, OUT FVec3& pNormal, OUT float& pDepth)
	{
		pNormal = FVec3::ZeroVector();
		pDepth = std::numeric_limits<float>::max();

		auto fn = [&](FVec3 edge) -> bool {
			FVec3 axis = FVec3(-edge.y, edge.x, 0);

			float min, max, min2, max2;
			ProjectVertices(pA, axis, min, max);
			ProjectSphere(pSphereLoc, pRadius, axis, min2, max2);

			if (min >= max2 || min2 >= max)
				return false;

			float axisDepth = SMath::Min(max2 - min, max - min2);
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
				auto distSqr = SMath::DistSqr(pA[i], pSphereLoc);
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

		if (SMath::Dot(pSphereLoc - SMath::Mean(pA), pNormal) < 0.f)
		{
			pNormal = pNormal * -1.f;
		}

		return true;
	}

	void SPhysyicsHandler::ClearOverlap(FVec3& pLHS, FVec3& pRHS, FVec3 pNormal, float pDepth)
	{
		pLHS -= pNormal * pDepth / 2.f;
		pRHS += pNormal * pDepth / 2.f;
	}

	void SPhysyicsHandler::HandleLinearCollisionResponse(FVec3& pVelocity1, FVec3& pVelocity2, float pMass1, float pMass2, FVec3 pNormal, float pRestitution /*= 1.f*/)
	{
		float j = -(1.f + pRestitution) * SMath::Dot((pVelocity2 - pVelocity1), pNormal) / (1 / pMass1 + 1 / pMass2);

		pVelocity1 -= pNormal * j / pMass1;
		pVelocity2 += pNormal * j / pMass2;
	}

	void SPhysyicsHandler::HandleAngularCollisionResponse(FVec3& pAngularVelocity1, FVec3& pAngularVelocity2, float pMass1, float pMass2, FVec3 pNormal, float pRestitution /*= 1.f*/)
	{

	}

	void SPhysyicsHandler::UpdateFrameLinearVelocity(FTransform2D& pTrans, const FVec3& pVelocity, float pDeltaTime)
	{
		pTrans.Location += pVelocity * pDeltaTime;
	}

	void SPhysyicsHandler::UpdateFrameAngularVelocity(FTransform2D& pTrans, const FVec3& pAngularVelocity, float pDeltaTime)
	{
		pTrans.Rotation += pAngularVelocity.x * pDeltaTime;
	}

	void SPhysyicsHandler::UpdateFrameAcceleration(FVec3& pVelocity, const FVec3& pAcceleration, float pDeltaTime)
	{
		pVelocity += pAcceleration * pDeltaTime;
	}

	void SPhysyicsHandler::UpdateFrameAngularAcceleration(FVec3& pAngularVelocity, const FVec3& pAngularAcceleration, float pDeltaTime)
	{
		pAngularVelocity += pAngularAcceleration * pDeltaTime;
	}

}