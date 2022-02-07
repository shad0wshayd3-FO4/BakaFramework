#pragma once

#include "Forms/Forms.h"

namespace Events::Handlers
{
	class CurrentRadiationSourceCountHandler :
		public RE::BSTValueEventSink<RE::CurrentRadiationSourceCount>
	{
	public:
		[[nodiscard]] static CurrentRadiationSourceCountHandler* GetSingleton()
		{
			static CurrentRadiationSourceCountHandler singleton;
			return std::addressof(singleton);
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::CurrentRadiationSourceCount& a_event, RE::BSTEventSource<RE::CurrentRadiationSourceCount>*) override
		{
			const RE::BSAutoLock locker{ dataLock };
			if (auto radiationSourceCount = Forms::RadiationSourceCount_DO->GetForm<RE::ActorValueInfo>(); radiationSourceCount)
			{
				auto value{ a_event.optionalValue.value_or(0) };
				RE::PlayerCharacter::GetSingleton()->SetBaseActorValue(*radiationSourceCount, static_cast<float>(value));
			}

			eventDataStruct.eventReceived = true;
			return RE::BSEventNotifyControl::kContinue;
		}
	};
}
