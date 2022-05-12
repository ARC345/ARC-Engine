#pragma once

#include "ECS_Defines.h"
#include <vector>
#include "tsl\ordered_set.h"
#include "tsl\sparse_map.h"

#include <algorithm>

namespace ARC::ECS
{
	template<typename TComponent>
	struct SPool
	{
		std::vector<TComponent> Packed;
		std::vector<EntityIndex_t> Entities;
		std::vector<size_t> Sparse;

		void Grow(size_t pNewCap) { Sparse.resize(pNewCap, InvalidEntityID); }
	
		TComponent& GetComponent(EntityIndex_t pI) const { return Packed[Sparse[pI]]; }
		TComponent& GetComponent(EntityIndex_t pI) { return Packed[Sparse[pI]]; }
		EntityIndex_t GetComponentOwner(const TComponent& _) const { return Entities[Entities.at(size_t(*_-Packed.begin()))]; }

		template <typename ... TArgs>
		TComponent& AddComponent(EntityIndex_t pEID, TArgs&& ... pXs) {
			Sparse[pEID] = Packed.size();
			Entities.push_back(pEID);
			return Packed.emplace_back(ECS_FWD(pXs)...);
		}

		void RemoveComponent(EntityIndex_t pEID)
		{
			if (Packed.end() == Packed.begin()+Sparse[pEID])
			{
				Sparse[pEID] = InvalidEntityID;
				Packed.pop_back();
				Entities.pop_back();
			}
			else
			{
				auto& curSparse = Sparse[pEID];
				auto lastEntity = Entities.back();
				std::swap(Packed[curSparse], Packed.back());
				std::swap(Entities[curSparse], Entities.back());

				Sparse[lastEntity] = curSparse;
				curSparse = InvalidEntityID;

				Packed.pop_back();
				Entities.pop_back();
			}	
		}
	};
}