// Copyright (c) 2015 Vittorio Romeo
// License: AFL 3.0 | https://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <memory>
#include "ARC/Ecs/ECS_Component.h"
#include <bitset>
#include <map>
#include <utility>
#include <variant>

namespace ARC::ECS {
	namespace Impl
	{
		template <typename TSettings>
		struct SEntity
		{
			using tSettings = TSettings;
			using tBitset = typename tSettings::tBitset;
			DataIndex_t DataIndex;
			HandleDataIndex_t HandleDataIndex;
			tBitset Bitset;
			uint8_t bAlive : 1;
		};

		struct SHandleData
		{
			EntityIndex_t EntityIndex;
			Counter_t Counter;
		};
		struct SHandle
		{
			HandleDataIndex_t HandleDataIndex;
			Counter_t Counter;
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
			inline SPool<T>& Get() {
				return std::get<SPool<T>>(m_Pools);
			}
		};
	}

	template <typename... Ts> using Signature = MPL::TypeList<Ts...>;
	template <typename... Ts> using SignatureList = MPL::TypeList<Ts...>;
	template <typename... Ts> using ComponentList = MPL::TypeList<Ts...>;
	template <typename... Ts> using TagList = MPL::TypeList<Ts...>;

	namespace Impl
	{
		template <typename TSettings> struct SSignatureBitsets;
		template <typename TSettings> class SSignatureBitsetsStorage;
	}

	template <typename TComponentList, typename TTagList, typename TSignatureList>
	struct SSettings
	{
		using tComponentList = typename TComponentList::TypeList;
		using tTagList = typename TTagList::TypeList;
		using tSignatureList = typename TSignatureList::TypeList;
		using tThisType = SSettings<tComponentList, tTagList, tSignatureList>;

		using tSignatureBitsets = Impl::SSignatureBitsets<tThisType>;
		using tSignatureBitsetsStorage = Impl::SSignatureBitsetsStorage<tThisType>;

		template <typename T>
		static constexpr bool IsComponent() noexcept { return MPL::Contains<T, tComponentList>{}; }
		template <typename T> 
		static constexpr bool IsTag() noexcept { return MPL::Contains<T, tTagList>{}; }
		template <typename T>
		static constexpr bool IsSignature() noexcept { return MPL::Contains<T, tSignatureList>{}; }
		static constexpr std::size_t GetComponentCount() noexcept { return MPL::size<tComponentList>(); }
		static constexpr std::size_t GetTagCount() noexcept { return MPL::size<tTagList>(); }
		static constexpr std::size_t GetSignatureCount() noexcept { return MPL::size<tSignatureList>();	}

		template <typename T> 
		static constexpr std::size_t GetComponentID() noexcept { return MPL::IndexOf<T, tComponentList>{}; }
		template <typename T> 
		static constexpr std::size_t GetTagID() noexcept { return MPL::IndexOf<T, tTagList>{}; }
		template <typename T> 
		static constexpr std::size_t GetSignatureID() noexcept { return MPL::IndexOf<T, tSignatureList>{}; }

		using tBitset = std::bitset<GetComponentCount() + GetTagCount()>;

		template <typename T>
		static constexpr std::size_t GetComponentBit() noexcept { return GetComponentID<T>(); }
		template <typename T>
		static constexpr std::size_t GetTagBit() noexcept { return GetComponentCount() + GetTagID<T>();	}
	};

	namespace Impl
	{
		template <typename TSettings>
		struct SSignatureBitsets
		{
			using tSettings = TSettings;
			using tThisType = SSignatureBitsets<tSettings>;
			using tSignatureList = typename tSettings::tSignatureList;
			using tBitset = typename tSettings::tBitset;

			using tBitsetStorage = MPL::Tuple<MPL::Repeat<tSettings::GetSignatureCount(), tBitset>>;

			template <typename T>
			using IsComponentFilter = std::integral_constant<bool, tSettings::template IsComponent<T>()>;
			template <typename T>
			using IsTagFilter =	std::integral_constant<bool, tSettings::template IsTag<T>()>;

			template <typename TSignature>
			using tSignatureComponents = MPL::Filter<IsComponentFilter, TSignature>;
			template <typename TSignature>
			using tSignatureTags = MPL::Filter<IsTagFilter, TSignature>;
		};

		template <typename TSettings>
		class SSignatureBitsetsStorage
		{
		private:
			using tSettings = TSettings;
			using tSignatureBitsets = typename tSettings::tSignatureBitsets;
			using tSignatureList = typename tSignatureBitsets::tSignatureList;
			using tBitsetStorage = typename tSignatureBitsets::tBitsetStorage;

			tBitsetStorage m_Storage;

		public:
			template <typename T>
			auto& GetSignatureBitset() noexcept
			{
				static_assert(tSettings::template IsSignature<T>(), "");
				return std::get<tSettings::template GetSignatureID<T>()>(m_Storage);
			}
			template <typename T>
			const auto& GetSignatureBitset() const noexcept
			{
				static_assert(tSettings::template IsSignature<T>(), "");
				return std::get<tSettings::template GetSignatureID<T>()>(m_Storage);
			}

		private:
			template <typename T>
			void InitializeBitset() noexcept
			{
				auto& b(this->GetSignatureBitset<T>());

				using tSignatureComponents = typename tSignatureBitsets::template tSignatureComponents<T>;
				using tSignatureTags = typename tSignatureBitsets::template tSignatureTags<T>;

				MPL::forTypes<tSignatureComponents>([this, &b](auto t)
					{
						b[tSettings::template GetComponentBit<ECS_TYPE(t)>()] = true;
					});

				MPL::forTypes<tSignatureTags>([this, &b](auto t)
					{
						b[tSettings::template GetTagBit<ECS_TYPE(t)>()] = true;
					});
			}

		public:
			SSignatureBitsetsStorage() noexcept
			{
				MPL::forTypes<tSignatureList>([this](auto t)
					{
						this->InitializeBitset<ECS_TYPE(t)>();
					});
			}
		};
	}

	template <typename TSettings>
	class CManager
	{
	private:
		// Type aliases.
		using tSettings = TSettings;
		using tThisType = CManager<tSettings>;
		using tBitset = typename tSettings::tBitset;
		using tEntity = Impl::SEntity<tSettings>;
		using tSignatureBitsetsStorage = Impl::SSignatureBitsetsStorage<tSettings>;
		using tComponentStorage = Impl::CComponentStorage<tSettings>;

	public:
		using Handle = Impl::SHandle;

	private:
		using HandleData = Impl::SHandleData;

		std::size_t m_Capacity{ 0 };
		std::size_t m_Size{ 0 };
		std::size_t m_SizeNext{ 0 };
		std::vector<tEntity> m_Entities;
		std::vector<HandleData> m_HandleData;
		
		tSignatureBitsetsStorage m_SignatureBitsets;
		tComponentStorage m_Components;

		void GrowTo(std::size_t _NewCapacity)
		{
			ARC_CORE_ASSERT(_NewCapacity > m_Capacity);
			m_Entities.resize(_NewCapacity);
			m_Components.Grow(_NewCapacity);
			m_HandleData.resize(_NewCapacity);
			for (auto i(m_Capacity); i < _NewCapacity; ++i)
			{
				auto& e(m_Entities[i]);
				auto& h(m_HandleData[i]);

				e.DataIndex = i;
				e.Bitset.reset();
				e.bAlive = false;
				e.HandleDataIndex = i;
				
				h.Counter = 0;
				h.EntityIndex = i;
			}

			m_Capacity = _NewCapacity;
		}
		void GrowIfNeeded()
		{
			if (m_Capacity > m_SizeNext) return;
			GrowTo((m_Capacity + 10) * 2);
		}

		auto& GetEntity(EntityIndex_t _I) noexcept
		{
			ARC_CORE_ASSERT(m_SizeNext > _I);
			return m_Entities[_I];
		}
		const auto& GetEntity(EntityIndex_t _I) const noexcept
		{
			ARC_CORE_ASSERT(m_SizeNext > _I);
			return m_Entities[_I];
		}

		auto& GetHandleData(HandleDataIndex_t _I) noexcept
		{
			ARC_CORE_ASSERT(m_HandleData.size() > _I);
			return m_HandleData[_I];
		}
		const auto& GetHandleData(HandleDataIndex_t _I) const noexcept
		{
			ARC_CORE_ASSERT(m_HandleData.size() > _I);
			return m_HandleData[_I];
		}

		inline auto& GetHandleData(EntityIndex_t _I) noexcept
		{
			return GetHandleData(GetEntity(_I).HandleDataIndex);
		}
		inline const auto& GetHandleData(EntityIndex_t _I) const noexcept
		{
			return GetHandleData(GetEntity(_I).HandleDataIndex);
		}

		auto& GetHandleData(const Handle& _I) noexcept { return GetHandleData(_I.HandleDataIndex); }
		const auto& GetHandleData(const Handle& _I) const noexcept { return GetHandleData(_I.HandleDataIndex); }

	public:
		CManager() { GrowTo(100); }
		auto IsHandleValid(Handle _I) const noexcept { return _I.Counter == GetHandleData(_I).Counter; }
		auto GetEntityIndex(const Handle& _I) const noexcept
		{
			ARC_CORE_ASSERT(IsHandleValid(_I));
			return GetHandleData(_I).EntityIndex;
		}
		auto IsAlive(EntityIndex_t _I) const noexcept
		{
			return GetEntity(_I).bAlive;
		}
		void Kill(EntityIndex_t _I) noexcept { GetEntity(_I).bAlive = true; }
		void Kill(const Handle& _I) noexcept { Kill(GetEntityIndex(_I)); }
		
		template <typename T>
		auto HasTag(EntityIndex_t _I) const noexcept
		{
			static_assert(tSettings::template IsTag<T>(), "");
			return GetEntity(_I).Bitset[tSettings::template GetTagBit<T>()];
		}
		template <typename T>
		auto HasTag(const Handle& _I) const noexcept
		{
			return HasTag<T>(GetEntityIndex(_I));
		}

		template <typename T>
		void AddTag(EntityIndex_t _I) noexcept
		{
			static_assert(tSettings::template IsTag<T>(), "");
			GetEntity(_I).Bitset[tSettings::template GetTagBit<T>()] = true;
		}
		template <typename T>
		void AddTag(const Handle& _I) noexcept
		{
			AddTag<T>(GetEntityIndex(_I));
		}
		
		template <typename T>
		void RemoveTag(EntityIndex_t _I) noexcept
		{
			static_assert(tSettings::template IsTag<T>(), "");
			GetEntity(_I).Bitset[tSettings::template GetTagBit<T>()] = false;
		}
		template <typename T>
		void RemoveTag(const Handle& _I) noexcept
		{
			RemoveTag<T>(GetEntityIndex(_I));
		}
		
		template <typename T>
		auto HasComponent(EntityIndex_t _I) const noexcept
		{
			static_assert(tSettings::template IsComponent<T>(), "");
			return GetEntity(_I).Bitset[tSettings::template GetComponentBit<T>()];
		}
		template <typename T>
		auto HasComponent(const Handle& _I) const noexcept
		{
			HasComponent<T>(GetEntityIndex(_I));
		}

		template <typename T, typename... TArgs>
		auto& AddComponent(EntityIndex_t _I, TArgs&&... mXs) noexcept
		{
			static_assert(tSettings::template IsComponent<T>());

			auto& e(GetEntity(_I));
			e.Bitset[tSettings::template GetComponentBit<T>()] = true;

			auto& c(m_Components.Get<T>().AddComponent(_I, e.DataIndex, ECS_FWD(mXs)...));
			
			return c;
		}
		template <typename T, typename... TArgs>
		auto& AddComponent(const Handle& _I, TArgs&&... mXs) noexcept
		{
			return AddComponent<T>(GetEntityIndex(_I), ECS_FWD(mXs)...);
		}
		template <typename T>
		auto& GetComponent(EntityIndex_t _I) noexcept {
			static_assert(tSettings::template IsComponent<T>());
			ARC_CORE_ASSERT(HasComponent<T>(_I));
			return m_Components.Get<T>().GetComponent(GetEntity(_I).DataIndex);
		}
		template <typename ... Ts>
		auto GetComponents(EntityIndex_t _I) noexcept {
			//static_assert(tSettings::template IsComponent<Ts>()...);
			//ARC_CORE_ASSERT(HasComponent<Ts>(_I)...);
			//

			auto di(GetEntity(_I).DataIndex);

			return std::tuple<Ts...>(m_Components.Get<Ts>().GetComponent(di)...);
		}
		
		template <typename T>
		auto& GetComponent(const Handle& _I) noexcept {
			return GetComponent<T>(GetEntityIndex(_I));
		}

		template <typename ... Ts>
		auto GetComponents(const Handle& _I) noexcept {
			return GetComponents<Ts...>(GetEntityIndex(_I));
		}

		template <typename T>
		std::vector<T>& GetAllComponentsOfClass() noexcept {
			static_assert(MPL::Contains<T, tSettings::tComponentList>::value);
			//return std::vector<T>;
			return m_Components.Get<T>().GetAllComponents();
		}

		template <typename T>
		void RemoveComponent(EntityIndex_t _I) noexcept
		{
			static_assert(tSettings::template IsComponent<T>(), "");
			GetEntity(_I).Bitset[tSettings::template GetComponentBit<T>()] = false;
		}
		template <typename T>
		void RemoveComponent(const Handle& _I) noexcept
		{
			RemoveComponent<T>(GetEntityIndex(_I));
		}
		auto CreateIndex()
		{
			GrowIfNeeded();
			EntityIndex_t freeIndex(m_SizeNext++);
			ARC_CORE_ASSERT(!IsAlive(freeIndex));
			auto& e(m_Entities[freeIndex]);
			e.bAlive = true;
			e.Bitset.reset();
			return freeIndex;
		}

		auto CreateHandle()
		{
			EntityIndex_t freeIndex(CreateIndex());
			ARC_CORE_ASSERT(IsAlive(freeIndex));
			
			auto& e(m_Entities[freeIndex]);
			auto& hd(m_HandleData[e.HandleDataIndex]);

			hd.EntityIndex = freeIndex;
			Handle h;

			h.HandleDataIndex = e.HandleDataIndex;
			h.Counter = hd.Counter;
			
			ARC_CORE_ASSERT(IsHandleValid(h));
			
			return h;
		}

		void Clear() noexcept
		{
			for (auto i(0u); i < m_Capacity; ++i)
			{
				auto& e(m_Entities[i]);
				auto& hd(m_HandleData[i]);

				e.DataIndex = i;
				e.Bitset.reset();
				e.bAlive = false;
				e.HandleDataIndex = i;

				hd.Counter = 0;
				hd.EntityIndex = i;
			}

			m_Size = m_SizeNext = 0;
		}

		void Refresh() noexcept
		{
			if (m_SizeNext == 0)
			{
				m_Size = 0;
				return;
			}
			m_Size = m_SizeNext = RefreshImpl();
		}

		template <typename T>
		auto MatchesSignature(EntityIndex_t _I) const noexcept
		{
			static_assert(tSettings::template IsSignature<T>());

			const auto& entityBitset(GetEntity(_I).Bitset);
			const auto& signatureBitset(m_SignatureBitsets.template GetSignatureBitset<T>());

			return (signatureBitset & entityBitset) == signatureBitset;
		}
		
		template <typename ... Ts>
		std::vector<EntityIndex_t> GetMatchingEntities() noexcept
		{
			using InTypes = MPL::TypeList<Ts...>;
			
			static_assert(InTypes::size > 0, "Please pass in more than 0 types");
			if constexpr (InTypes::size == 1)
			{
				return m_Components.Get<Ts...>().GetAllEntities();
			}
			
			bool bFirst = true;
			std::vector<EntityIndex_t> rval;
			MPL::forTypes<InTypes>([&](auto t) {
					auto& iX = m_Components.Get<ECS_TYPE(t)>().GetAllEntities();
					if (bFirst)
					{
						rval = iX; 
					}
					else
					{
						rval.erase(
							std::remove_if(rval.begin(), rval.end(), [&](size_t i) {
								return std::binary_search(iX.begin(), iX.end(), i);
							})
							, rval.end());
					}
				});
			return rval;
		}


		template <typename TF>
		void UpdateEntities(TF&& mFunction)
		{
			for (EntityIndex_t i{ 0 }; i < m_Size; ++i) mFunction(i);
		}
		
		template <typename T, typename TF>
		void UpdateMatchingEntities(TF&& mFunction)
		{
			static_assert(tSettings::template IsSignature<T>(), "");
			
			UpdateEntities([this, &mFunction](auto i)
				{
					if (MatchesSignature<T>(i))
						ExpandSignatureCall<T>(i, mFunction);
				});
		}

	private:
		//namespace Impl {
			template <typename... Ts>
			struct ExpandCallHelper;

			template <typename T, typename TF>
			void ExpandSignatureCall(EntityIndex_t mI, TF&& mFunction)
			{
				static_assert(tSettings::template IsSignature<T>(), "");
				using RequiredComponents = typename tSettings::tSignatureBitsets::
					template tSignatureComponents<T>;

				using Helper = MPL::Rename<ExpandCallHelper, RequiredComponents>;

				Helper::call(mI, *this, mFunction);
			}

			template <typename... Ts>
			struct ExpandCallHelper
			{
				template <typename TF>
				static void call(EntityIndex_t mI, tThisType& mMgr, TF&& mFunction)
				{
					auto di(mMgr.GetEntity(mI).DataIndex);

					mFunction(mI, mMgr.m_Components.Get<Ts>().GetComponent(di)...);
				}
			};
		//}
	private:
		void InvalidateHandle(EntityIndex_t _X) noexcept
		{
			auto& hd(m_HandleData[m_Entities[_X].HandleDataIndex]);
			++hd.Counter;
		}

		void RefreshHandle(EntityIndex_t _X) noexcept
		{
			auto& hd(m_HandleData[m_Entities[_X].HandleDataIndex]);
			hd.EntityIndex = _X;
		}
		auto RefreshImpl() noexcept
		{
			EntityIndex_t iD{ 0 }, iA{ m_SizeNext - 1 };

			while (true)
			{
				for (; true; ++iD)
				{
					if (iD > iA) return iD;
					if (!m_Entities[iD].bAlive) break;
				}

				for (; true; --iA)
				{
					if (m_Entities[iA].bAlive) break;
					InvalidateHandle(iA);
					if (iA <= iD) return iD;
				}

				ARC_CORE_ASSERT(m_Entities[iA].bAlive);
				ARC_CORE_ASSERT(!m_Entities[iD].bAlive);

				std::swap(m_Entities[iA], m_Entities[iD]);

				InvalidateHandle(iA);
				InvalidateHandle(iD);

				++iD;
				--iA;
			}

			return iD;
		}

	public:
		auto GetEntityCount() const noexcept { return m_Size; }
		auto GetCapacity() const noexcept { return m_Capacity; }
		auto GetSizeNext() const noexcept { return m_SizeNext; }
		auto GetPrintString() const noexcept {
			std::string x = fmt::format(
				"size: {}\n"
				"size next: {}\n"
				"capacity: {}\n",
				GetEntityCount(),
				GetSizeNext(),
				GetCapacity()
				);

			for (auto i(0u); i < GetSizeNext(); ++i)
			{
				x = fmt::format("{} {}", x, GetEntity(EntityIndex_t(i)).bAlive ? "A" : "D");
			}

			//os << "\n\n";
			return x;
		}
	};

	namespace IMPL {
		template<size_t N> struct ITRL_ComponentList { using type = MPL::TypeList<>; };
		template<size_t N> struct ITRL_SignatureList { using type = MPL::TypeList<>; };
		template<size_t N> struct ITRL_TagList { using type = MPL::TypeList<>; };
	}
	class ComponentCounterId : CounterId {};
	class SignatureCounterId : CounterId {};
	class TagCounterId : CounterId {};
}

#define ITRL_ECS_Var_GetName(x, y) COMBINE4(_ARC_ECS_CLASS_LIST_COUNTER_VAR_, y, _, x)

#define ITRL_ECS_Component(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Component) = HPR::CustomCounter<ECS::ComponentCounterId>::Next();
#define ITRL_ECS_Signature(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Signature) = HPR::CustomCounter<ECS::SignatureCounterId>::Next();
#define ITRL_ECS_Tag(x) static constexpr size_t ITRL_ECS_Var_GetName(x, Tag) = HPR::CustomCounter<ECS::TagCounterId>::Next();

#define RegisterComponent(x) \
		ITRL_ECS_Component(x); \
		template<> \
		struct ECS::IMPL::ITRL_ComponentList<ITRL_ECS_Var_GetName(x, Component)> { \
			using type = MPL::PushBack<ECS::IMPL::ITRL_ComponentList<ITRL_ECS_Var_GetName(x, Component)-1>::type, x>; \
		};
#define RegisterSignature(x) \
		ITRL_ECS_Signature(x); \
		template<> \
		struct ECS::IMPL::ITRL_SignatureList<ITRL_ECS_Var_GetName(x, Signature)> { \
			using type = MPL::PushBack<ECS::IMPL::ITRL_SignatureList<ITRL_ECS_Var_GetName(x, Signature)-1>::type, x>; \
		};
#define RegisterTag(x) \
		ITRL_ECS_Tag(x); \
		template<> \
		struct ECS::IMPL::ITRL_TagList<ITRL_ECS_Var_GetName(x, Tag)> { \
			using type = MPL::PushBack<ECS::IMPL::ITRL_TagList<ITRL_ECS_Var_GetName(x, Tag)-1>::type, x>; \
		};


namespace ARC
{
	using EntityID = ECS::EntityIndex_t;
}