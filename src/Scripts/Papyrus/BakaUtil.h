#pragma once

#include "Scripts/Papyrus/Shared/RegistrationMap.h"

namespace Papyrus::BakaUtil
{
	namespace detail
	{
		class PipboyLightEventHandler :
			public RE::BSTValueEventSink<RE::PipboyLightEvent>,
			public Papyrus::Shared::RegistrationMap
		{
		public:
			[[nodiscard]] static PipboyLightEventHandler* GetSingleton()
			{
				static PipboyLightEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
			{
				const RE::BSAutoLock lock{ dataLock };
				DispatchImpl(a_event.optionalValue.value_or(false));

				eventDataStruct.eventReceived = true;
				return RE::BSEventNotifyControl::kContinue;
			}

			virtual std::uint32_t GetRecordSig() { return 'PLEH'; }

			virtual std::uint32_t GetRecordVer() { return 2; }

			virtual const char* GetEventName() { return "OnPipboyLightEvent"; }
		};
	}

	constexpr auto SCRIPT_NAME{ "BakaUtil"sv };

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

	void RegisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventHandler::GetSingleton()->Register(a_this);
	}

	void UnregisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventHandler::GetSingleton()->Unregister(a_this);
	}
}
