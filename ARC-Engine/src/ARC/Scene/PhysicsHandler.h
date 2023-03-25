#pragma once

namespace ARC {
	class SPhysyicsHandler
	{
	public:
		static void ProjectVertices(std::vector<FVec3>& pA, FVec3 pAxis, OUT float& pMin, OUT float& pMax);
		static void ProjectSphere(FVec3 pLoc, float pRadius, FVec3 pAxis, OUT float& pMin, OUT float& pMax);

		static bool IsCollidingSphere(FVec3 pLoc1, FVec3 pLoc2, float pRadiusSum, OUT FVec3& pNormal, OUT float& pDepth);
		static bool IsCollidingSphere(FVec3 pLoc1, FVec3 pLoc2, float pRadiusSum);
		static bool IsCollidingPolygon2D(std::vector<FVec3>& pA, std::vector<FVec3>& pB, OUT FVec3& pNormal, OUT float& pDepth); //@TODO using 3d cause im lazy
		static bool IsCollidingPolygon2DVsSphere(std::vector<FVec3>& pA, FVec3 pSphereLoc, float pRadius, OUT FVec3& pNormal, OUT float& pDepth); //@TODO using 3d cause im lazy
		//static bool IsCollidingPolygon2D(std::vector<FVec2>& pA, std::vector<FVec2>& pB); 
		
		static void ClearOverlap(FVec3& pLHS, FVec3& pRHS, FVec3 pNormal, float pDepth);

		static void HandleLinearCollisionResponse(FVec3& pVelocity1, FVec3& pVelocity2, float pMass1, float pMass2, FVec3 pNormal, float pRestitution = 1.f);
		static void HandleAngularCollisionResponse(FVec3& pAngularVelocity1, FVec3& pAngularVelocity2, float pMass1, float pMass2, FVec3 pNormal, float pRestitution = 1.f);
		
		static void UpdateFrameLinearVelocity(FTransform2D& pTrans, const FVec3& pVelocity, float pDeltaTime);
		static void UpdateFrameAngularVelocity(FTransform2D& pTrans, const FVec3& pAngularVelocity, float pDeltaTime);
		static void UpdateFrameAcceleration(FVec3& pVelocity, const FVec3& pAcceleration, float pDeltaTime);
		static void UpdateFrameAngularAcceleration(FVec3& pAngularVelocity, const FVec3& pAngularAcceleration, float pDeltaTime);
	};
}