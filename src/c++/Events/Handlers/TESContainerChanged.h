#pragma once

#include "Forms/Forms.h"

namespace Events::Handlers
{
	class TESContainerChangedEventHandler :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		[[nodiscard]] static TESContainerChangedEventHandler* GetSingleton()
		{
			static TESContainerChangedEventHandler singleton;
			return std::addressof(singleton);
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent& a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override
		{
			if (a_event.newContainerFormID == 0x14 || a_event.oldContainerFormID == 0x14)
			{
				if (auto InventoryWeight = Forms::InventoryWeight_DO->GetForm<RE::ActorValueInfo>())
				{
					auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
					PlayerCharacter->SetBaseActorValue(
						*InventoryWeight,
						PlayerCharacter->GetWeightInContainer());
				}
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};
}
