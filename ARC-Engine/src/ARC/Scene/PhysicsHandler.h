#pragma once

namespace arc {
	class physyics_handler
	{
	public:
		static void ProjectVertices(std::vector<vec3>& pA, vec3 pAxis, OUT float& pMin, OUT float& pMax);
		static void ProjectSphere(vec3 pLoc, float pRadius, vec3 pAxis, OUT float& pMin, OUT float& pMax);

		static bool is_colliding_sphere(vec3 pLoc1, vec3 pLoc2, float pRadiusSum, OUT vec3& pNormal, OUT float& pDepth);
		static bool is_colliding_sphere(vec3 pLoc1, vec3 pLoc2, float pRadiusSum);
		static bool is_colliding_polygon2_d(std::vector<vec3>& pA, std::vector<vec3>& pB, OUT vec3& pNormal, OUT float& pDepth); //@TODO using 3d cause im lazy
		static bool is_colliding_polygon2_d_vs_sphere(std::vector<vec3>& pA, vec3 pSphereLoc, float pRadius, OUT vec3& pNormal, OUT float& pDepth); //@TODO using 3d cause im lazy
		//static bool IsCollidingPolygon2D(std::vector<vec2>& pA, std::vector<vec2>& pB); 
		
		static void ClearOverlap(vec3& pLHS, vec3& pRHS, vec3 pNormal, float pDepth);

		static void HandleLinearCollisionResponse(vec3& pVelocity1, vec3& pVelocity2, float pMass1, float pMass2, vec3 pNormal, float pRestitution = 1.f);
		static void HandleAngularCollisionResponse(vec3& pAngularVelocity1, vec3& pAngularVelocity2, float pMass1, float pMass2, vec3 pNormal, float pRestitution = 1.f);
		
		static void UpdateFrameLinearVelocity(transform_2d& pTrans, const vec3& pVelocity, float pDeltaTime);
		static void UpdateFrameAngularVelocity(transform_2d& pTrans, const vec3& pAngularVelocity, float pDeltaTime);
		static void UpdateFrameAcceleration(vec3& pVelocity, const vec3& pAcceleration, float pDeltaTime);
		static void UpdateFrameAngularAcceleration(vec3& pAngularVelocity, const vec3& pAngularAcceleration, float pDeltaTime);
	};
}