#pragma once

#include "Forms/Forms.h"

namespace Events::Handlers
{
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
			if (auto pipboyLightActive = Forms::PipboyLightActive_DO->GetForm<RE::ActorValueInfo>())
			{
				auto value{ a_event.optionalValue.value_or(false) ? 1.0f : 0.0f };
				RE::PlayerCharacter::GetSingleton()->SetBaseActorValue(*pipboyLightActive, value);
			}

			eventDataStruct.eventReceived = true;
			return RE::BSEventNotifyControl::kContinue;
		}
	};
}
