#pragma once

#include "Forms/Forms.h"

namespace Events::Handlers
{
	class PipboyLightEventHandler :
		public REX ::Singleton<PipboyLightEventHandler>,
		public RE::BSTValueEventSink<RE::PipboyLightEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
		{
			const RE::BSAutoLock locker{ dataLock };
			if (auto pipboyLightActive = Forms::PipboyLightActive)
			{
				auto value{ a_event.optionalValue.value_or(false) ? 1.0f : 0.0f };
				RE::PlayerCharacter::GetSingleton()->SetBaseActorValue(*pipboyLightActive, value);
			}

			eventDataStruct.eventReceived = true;
			return RE::BSEventNotifyControl::kContinue;
		}
	};
}
