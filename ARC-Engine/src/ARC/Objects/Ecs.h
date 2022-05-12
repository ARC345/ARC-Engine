#pragma once

#include <memory>
#include "ARC/Ecs/ECS_Component.h"
#include <bitset>
#include <map>
#include <utility>
#include <variant>
#include <stack>

namespace ARC::ECS
{
	template <typename TComponentList, typename TTagList, typename TSignatureList>
	struct SSettings
	{
		using tComponentList = typename TComponentList::TypeList;
		using tTagList = typename TTagList::TypeList;
		using tThisType = SSettings<tComponentList, tTagList, TSignatureList>;

#define _SettingsFunc static constexpr

		template <typename T>
		_SettingsFunc bool IsComponent() noexcept { return MPL::Contains<T, tComponentList>{}; }
		template <typename T>
		_SettingsFunc bool IsTag() noexcept { return MPL::Contains<T, tTagList>{}; }
		_SettingsFunc std::size_t GetComponentCount() noexcept { return MPL::size<tComponentList>(); }
		_SettingsFunc std::size_t GetTagCount() noexcept { return MPL::size<tTagList>(); }

		template <typename T>
		_SettingsFunc std::size_t GetComponentID() noexcept { return MPL::IndexOf<T, tComponentList>{}; }
		template <typename T>
		_SettingsFunc std::size_t GetTagID() noexcept { return MPL::IndexOf<T, tTagList>{}; }

		using tBitset = std::bitset<GetComponentCount() + GetTagCount()>;

		template <typename T>
		_SettingsFunc std::size_t GetComponentBit() noexcept { return GetComponentID<T>(); }
		template <typename T>
		_SettingsFunc std::size_t GetTagBit() noexcept { return GetComponentCount() + GetTagID<T>(); }
#undef _SettingsFunc 
	};

	template <typename TSettings>
	class CComponentStorage
	{
	private:
		using tSettings = TSettings;
		using tComponentList = typename tSettings::tComponentList;

		template <typename... Ts>
		using tPools = std::tuple<SPool<Ts>...>;

		MPL::Rename<tPools, tComponentList> m_Pools;
	public:
		void Grow(std::size_t pNewCapacity)
		{
			Utils::forTuple(
				[this, pNewCapacity](auto& v)
				{
					v.Grow(pNewCapacity);
				},
				m_Pools);
		}

		template<typename T>
		inline SPool<T>& Get() { return std::get<SPool<T>>(m_Pools); }
		template<typename T>
		inline const SPool<T>& Get() const { return std::get<SPool<T>>(m_Pools); }
	};

	template <typename... Ts> using ComponentList = MPL::TypeList<Ts...>;
	template <typename... Ts> using TagList = MPL::TypeList<Ts...>;

	template <typename TSettings>
	class CManager
	{
	private:
		// Type aliases.
		using tSettings = TSettings;
		using tThisType = CManager<tSettings>;
		using tBitset = typename tSettings::tBitset;
		using tComponentStorage = CComponentStorage<tSettings>;

	public:
		CManager() { Resize(10); }

		size_t GetCapacity() const { return m_Capacity; }
		size_t GetSize() const { return m_Size; }
		size_t GetEntityCount() const { return m_EntityCount; }
		size_t GetEmptyCount() const { return m_Size - m_EntityCount; }
		
		EntityIndex_t FindFirstDead(EntityIndex_t pOffset = 0) const;
		EntityIndex_t FindFirstAlive() const;

		template<typename T> static constexpr bool IsComponent() { return tSettings::IsComponent<T>(); }

		template<typename T> bool HasComponent(EntityIndex_t pEnt) const { return m_Components.Get<T>().Sparse[pEnt] != InvalidEntityID; }
		template<typename T> T& GetComponent(EntityIndex_t pEnt) { return m_Components.Get<T>().GetComponent(pEnt); }
		
		template<typename T> const std::vector<T>& GetComponents() const { return m_Components.Get<T>().Packed; }
		template<typename T> std::vector<T>& GetComponents() { return m_Components.Get<T>().Packed; }
		
		template<typename ... Ts>
		std::vector<EntityIndex_t> Filter() {
			using TL = MPL::TypeList<Ts...>;
			static_assert(TL::size > 0, "Please pass in more than 0 types");
			if constexpr (TL::size == 1) return m_Components.Get<Ts...>().Entities;

			std::vector<EntityIndex_t> rval;
			bool bFirst = true;
			MPL::forTypes<TL>([&](auto t) {
				using tType = ECS_TYPE(t);
				auto& Pool = m_Components.Get<tType>();
				if (bFirst) { rval = Pool.Entities; bFirst = false; }
				else
				{
					rval.erase(
						std::remove_if(
								rval.begin(),
								rval.end(),
								[&](size_t i) { return std::find(Pool.Entities.begin(), Pool.Entities.end(), i)==Pool.Entities.end(); }
								), rval.end());
				}
			});

			return rval;
		}

		template<typename T, typename T2, typename ... Ts>
		std::vector<std::tuple<EntityIndex_t, T&, T2&, Ts&...>> FilterComponents() {
			using TL = MPL::TypeList<T, T2, Ts...>;
			using TupleType = std::tuple<EntityIndex_t, T&, T2&, Ts&...>;
			auto Enitites = Filter<T, T2, Ts...>();

			std::vector<TupleType> rval;

			for (auto& _ : Enitites)
			{
				rval.push_back(TupleType(_, GetComponent<T>(_), GetComponent<T2>(_), GetComponent<Ts>(_)...));
			}

			return rval;
		}

		bool IsValidIndex(EntityIndex_t pEnt) const { return m_Data[pEnt] != InvalidEntityID; }
		bool Contains(EntityIndex_t pEnt) const { return m_Data[pEnt] < m_Size; }
		bool IsAlive(EntityIndex_t pEnt) const { return m_Data[pEnt] != DeadEntityID && Contains(pEnt); }

		void GrowIfNeeded() { if (m_Capacity > m_Size) return; Resize((m_Capacity + 10) * 2); }
		template<typename T, typename ... Ts> T& AddComponent(EntityIndex_t pEnt, Ts&&... pArgs) {
			static_assert(IsComponent<T>());
			ARC_CORE_ASSERT(IsAlive(pEnt));
			return m_Components.Get<T>().AddComponent(pEnt, ECS_FWD(pArgs)...);
		}
		template<typename T> void RemoveComponent(EntityIndex_t pEnt) {
			static_assert(IsComponent<T>());
			ARC_CORE_ASSERT(IsAlive(pEnt));
			ARC_CORE_ASSERT(HasComponent<T>(pEnt));
			return m_Components.Get<T>().RemoveComponent(pEnt);
		}
		void Resize(size_t pNewSize) { 
			m_Data.resize(pNewSize, (EntityIndex_t)InvalidEntityID);
			MPL::forTypes<tSettings::tComponentList>([&](auto t){
				m_Components.Get<ECS_TYPE(t)>().Grow(pNewSize);
			});
		};

		void Kill(EntityIndex_t pI) {
			ARC_CORE_ASSERT(IsValidIndex(pI));
			ARC_CORE_ASSERT(Contains(pI));
			ARC_CORE_ASSERT(IsAlive(pI));

			MPL::forTypes<tSettings::tComponentList>([&](auto t) {				
				using tType = ECS_TYPE(t);
				if (HasComponent<tType>(pI))
				{
					RemoveComponent<tType>(pI);
				}
			});
			m_Data[pI] = DeadEntityID;
		}

		EntityIndex_t CreateEntity() {
			GrowIfNeeded();
			
			EntityIndex_t freeIndex;
			if (m_SelectedEmpty == InvalidEntityID)
				freeIndex = m_Size++;
			else
			{
				freeIndex = m_SelectedEmpty;
				m_SelectedEmpty = FindFirstDead(m_SelectedEmpty);
			}

			ARC_CORE_ASSERT(!IsAlive(freeIndex));
			m_SelectedEmpty = InvalidEntityID;
			m_Data[freeIndex] = freeIndex;
			m_EntityCount++;
			return freeIndex;
		}

		template <typename TF>
		void UpdateEntities(TF&& pFunction) {
			UpdateEntitiesRaw([&](EntityIndex_t i){ if(IsAlive(i)) pFunction(i); });
		}
		template <typename TF>
		void UpdateEntitiesRaw(TF&& pFunction) {
			for (EntityIndex_t i = (EntityIndex_t)0; i < (EntityIndex_t)m_Size; ++i)
				pFunction(i);
		}

	public:

	private:
		size_t m_Capacity	 = 0;
		size_t m_Size		 = 0;
		size_t m_EntityCount = 0;
		
		EntityIndex_t m_SelectedEmpty = (EntityIndex_t)InvalidEntityID;

		std::vector<EntityIndex_t> m_Data;

		tComponentStorage m_Components;
	};

	template <typename TSettings>
	EntityIndex_t CManager<TSettings>::FindFirstDead(EntityIndex_t pOffset /*= 0*/) const
	{
		for (EntityIndex_t i = pOffset; i < m_Size; ++i)
			if (Contains(i) && !IsAlive(i)) return i;
		return (EntityIndex_t)InvalidEntityID;
	}
	template <typename TSettings>
	EntityIndex_t CManager<TSettings>::FindFirstAlive() const
	{
		for (EntityIndex_t i = 0; i < m_Size; ++i)
			if (IsAlive(i)) return i;
		return InvalidEntityID;
	}

	namespace IMPL {
		template<size_t N> struct ITRL_ComponentList { using type = MPL::TypeList<>; };
		template<size_t N> struct ITRL_SignatureList { using type = MPL::TypeList<>; };
		template<size_t N> struct ITRL_TagList { using type = MPL::TypeList<>; };
	}
	class ComponentCounterId : CounterId {};
	class SignatureCounterId : CounterId {};
	class TagCounterId : CounterId {};

	template<typename>
	struct ComponentName {
		static const char* Get() { return "unknown"; }
	};
}

#define DEF_COMPONENT_NAME(X) template<> struct ECS::ComponentName<X> { static const char* Get() { return #X; } }
#define ITRL_ECS_Var_GetName(x, y) COMBINE4(_ARC_ECS_CLASS_LIST_COUNTER_VAR_, y, _, x)

#define ITRL_ECS_Component(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Component) = HPR::CustomCounter<ECS::ComponentCounterId>::Next();
#define ITRL_ECS_Signature(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Signature) = HPR::CustomCounter<ECS::SignatureCounterId>::Next();
#define ITRL_ECS_Tag(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Tag) = HPR::CustomCounter<ECS::TagCounterId>::Next();

#define RegisterComponent(x) \
		ITRL_ECS_Component(x); \
		template<> \
		struct ECS::IMPL::ITRL_ComponentList<ITRL_ECS_Var_GetName(x, Component)> { \
			using type = MPL::PushBack<ECS::IMPL::ITRL_ComponentList<ITRL_ECS_Var_GetName(x, Component)-1>::type, x>; \
		};\
		DEF_COMPONENT_NAME(x);

namespace ARC
{
	using EntityID = ECS::EntityIndex_t;
}