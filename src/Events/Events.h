#pragma once

#include "Forms/Forms.h"
#include "Serialization/Serialization.h"

namespace Events
{
	namespace
	{
		class CurrentRadiationSourceCount :
			public REX::TSingleton<CurrentRadiationSourceCount>,
			public RE::BSTValueEventSink<RE::CurrentRadiationSourceCount>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::CurrentRadiationSourceCount& a_event, RE::BSTEventSource<RE::CurrentRadiationSourceCount>*) override
			{
				const RE::BSAutoLock locker{ dataLock };
				if (Forms::RadiationSourceCount)
				{
					auto value{ a_event.optionalValue.value_or(0) };
					if (auto player = RE::PlayerCharacter::GetSingleton())
						player->SetBaseActorValue(*Forms::RadiationSourceCount, static_cast<float>(value));
					Serialization::EventManager::GetSingleton()->radiationCount.QueueEvent(static_cast<std::int32_t>(value));
				}

				eventDataStruct.eventReceived = true;
				return RE::BSEventNotifyControl::kContinue;
			}
		};

		class PipboyLightEvent :
			public REX::TSingleton<PipboyLightEvent>,
			public RE::BSTValueEventSink<RE::PipboyLightEvent>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
			{
				const RE::BSAutoLock locker{ dataLock };
				if (Forms::PipboyLightActive)
				{
					auto value{ a_event.optionalValue.value_or(false) };
					if (auto player = RE::PlayerCharacter::GetSingleton())
						player->SetBaseActorValue(*Forms::PipboyLightActive, static_cast<float>(value));
					Serialization::EventManager::GetSingleton()->pipboyLight.QueueEvent(value);
				}

				eventDataStruct.eventReceived = true;
				return RE::BSEventNotifyControl::kContinue;
			}
		};

		class ItemCraftedHandler :
			public REX::TSingleton<ItemCraftedHandler>,
			public RE::BSTEventSink<RE::ItemCrafted::Event>,
			public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			virtual ~ItemCraftedHandler()
			{
				m_refr.reset();
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::ItemCrafted::Event& a_event, RE::BSTEventSource<RE::ItemCrafted::Event>*) override
			{
				if (auto storyEventManager = RE::BGSStoryEventManager::GetSingleton())
				{
					RE::BGSLocation* location{ nullptr };
					if (auto parentCell = m_refr ? m_refr->GetParentCell() : nullptr)
					{
						location = parentCell->GetLocation();
					}

					auto item = a_event.recipe ? a_event.recipe->GetCreatedItem() : nullptr;
					storyEventManager->AddEvent(RE::BGSCraftItemEvent{ m_refr.get(), location, item });
				}

				return RE::BSEventNotifyControl::kContinue;
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
			{
				if (a_event.menuName == "ExamineMenu"sv || a_event.menuName == "CookingMenu"sv || a_event.menuName == "RobotModMenu"sv)
				{
					if (a_event.opening)
					{
						if (auto UI = RE::UI::GetSingleton())
						{
							auto menu = static_cast<RE::WorkbenchMenuBase*>(UI->GetMenu(a_event.menuName).get());
							if (menu && menu->workbenchRef)
							{
								m_refr.reset(menu->workbenchRef.get());
							}
						}
					}
					else
					{
						m_refr.reset();
					}
				}

				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			RE::BSTSmartPointer<RE::TESObjectREFR, RE::BSTSmartPointerGamebryoRefCount> m_refr;
		};
	}

	void Register()
	{
		if (auto eventSource = RE::CurrentRadiationSourceCount::GetEventSource())
			eventSource->RegisterSink(CurrentRadiationSourceCount::GetSingleton());

		if (auto eventSource = RE::PipboyLightEvent::GetEventSource())
			eventSource->RegisterSink(PipboyLightEvent::GetSingleton());
		
		if (auto ui = RE::UI::GetSingleton())
		{
			RE::ItemCrafted::RegisterSink(ItemCraftedHandler::GetSingleton());
			ui->RegisterSink<RE::MenuOpenCloseEvent>(ItemCraftedHandler::GetSingleton());
		}

		REX::DEBUG("Registered Event Handlers."sv);
	}
}
