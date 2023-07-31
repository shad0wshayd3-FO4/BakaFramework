#pragma once

#include "SteamAPI/SteamAPI.h"

//#include "Workshop/Workshop.h"

namespace Papyrus::BakaUtil
{
	namespace detail
	{
		class PipboyLightEventMap
		{
		public:
			static void Register(const RE::BSScript::Variable* a_variable)
			{
				if (!a_variable->is<RE::BSScript::Object>())
				{
					return;
				}

				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
					if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
					{
						return;
					}

					const RE::BSAutoLock lock{ spin };
					GameVM->handlePolicy.PersistHandle(object->GetHandle());
					map.emplace(
						object->GetHandle(),
						object->type
							? object->type->GetName()
							: "");
				}
			}

			static void Unregister(const RE::BSScript::Variable* a_variable)
			{
				if (!a_variable->is<RE::BSScript::Object>())
				{
					return;
				}

				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
					if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
					{
						return;
					}

					const RE::BSAutoLock lock{ spin };
					GameVM->handlePolicy.ReleaseHandle(object->GetHandle());
					map.erase(object->GetHandle());
				}
			}

			static void Dispatch(bool a_value)
			{
				auto GameVM = RE::GameVM::GetSingleton();
				if (!GameVM)
				{
					return;
				}

				auto VM = GameVM->GetVM();
				if (!VM)
				{
					return;
				}

				const RE::BSAutoLock lock{ spin };
				for (auto& iter : map)
				{
					VM->DispatchMethodCall(
						iter.first,
						iter.second,
						"OnPipboyLightEvent"sv,
						nullptr,
						a_value);
				}
			}

		private:
			inline static RE::BSSpinLock spin;
			inline static RE::BSTHashMap<std::uint64_t, const char*> map;
		};

		class PipboyLightEventHandler :
			public RE::BSTValueEventSink<RE::PipboyLightEvent>
		{
		public:
			[[nodiscard]] static PipboyLightEventHandler* GetSingleton()
			{
				static PipboyLightEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
			{
				const RE::BSAutoLock locker{ dataLock };
				PipboyLightEventMap::Dispatch(a_event.optionalValue.value_or(false));

				eventDataStruct.eventReceived = true;
				return RE::BSEventNotifyControl::kContinue;
			}
		};
	}

	constexpr auto SCRIPT_NAME{ "BakaUtil"sv };

	/*
	bool CreatePowerArmorToken(std::monostate, RE::TESObjectREFR* a_refr)
	{
		return Workshop::PlacementMode::CreateToken(a_refr);
	}

	bool HandlePowerArmorToken(std::monostate, RE::TESObjectREFR* a_refr)
	{
		return Workshop::PlacementMode::HandleToken(a_refr);
	}
	*/

	std::vector<RE::TESObjectREFR*> FilterReferencesByKeywords(
		std::monostate,
		std::vector<RE::TESObjectREFR*> a_refrs,
		std::vector<RE::BGSKeyword*> a_kywds,
		bool a_whitelist)
	{
		std::vector<RE::TESObjectREFR*> result;
		if (a_refrs.size() == 0 || a_kywds.size() == 0)
		{
			return result;
		}

		for (auto& refr : a_refrs)
		{
			if (refr)
			{
				for (auto& kywd : a_kywds)
				{
					if (kywd)
					{
						if (a_whitelist && refr->HasKeyword(kywd))
						{
							result.emplace_back();
						}
					}
				}
			}
		}

		return result;
	}

	void OpenWebPage(std::monostate, std::string_view a_url, bool a_fallback)
	{
		SteamAPI::OpenWebPage(a_url.data(), a_fallback);
	}

	void RegisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventMap::Register(a_this);
	}

	void UnregisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventMap::Unregister(a_this);
	}
}
