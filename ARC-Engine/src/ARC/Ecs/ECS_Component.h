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
		std::vector<size_t> Sparse; // @todo implement space saving stuff
		uint8_t bNeedsRefresh : 1;

		void Grow(size_t pNewCap)
		{
		}

		const std::vector<EntityIndex_t> GetAllEntities() {
			return Entities;
		}

		std::vector<TComponent>& GetAllComponents() { 
			return Packed;
		}
		TComponent& GetComponent(DataIndex_t pI) { 
			return Packed[Sparse[pI]];
		}
	
		template <typename ... TArgs>
		TComponent& AddComponent(EntityIndex_t pEID, DataIndex_t pDI, TArgs&& ... pXs) {
			if (Sparse.size()<pDI+1)
				Sparse.resize(pDI+1);
			
			Sparse[pDI] = Packed.size();
			Entities.push_back(pEID);
			bNeedsRefresh = 1u;
			return Packed.emplace_back(ECS_FWD(pXs)...);
		}
		void RemoveComponent(DataIndex_t _I) {
			auto index = Sparse[_I];
			Packed.erase(index);
			Entities.erase(index);
			Sparse.erase(_I);
		}
	};
}