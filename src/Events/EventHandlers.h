#pragma once

#include "Forms/Forms.h"

namespace Tasks
{
	class WeightUpdate :
		public F4SE::ITaskDelegate
	{
	public:
		WeightUpdate(RE::TESObjectREFR* a_refr) :
			m_refr(a_refr)
		{}

		virtual ~WeightUpdate()
		{
			m_refr.reset();
		}

		virtual void Run() override
		{
			auto refr = m_refr.get();
			if (refr)
			{
				if (auto inventoryWeight = Forms::InventoryWeight_DO->GetForm<RE::ActorValueInfo>(); inventoryWeight)
				{
					refr->SetBaseActorValue(*inventoryWeight, refr->GetWeightInContainer());
				}
			}
		}

	protected:
		RE::BSTSmartPointer<RE::TESObjectREFR, RE::BSTSmartPointerGamebryoRefCount> m_refr;
	};
}

namespace EventHandlers
{
	class CurrentRadiationSourceCountHandler :
		public RE::BSTValueEventSink<RE::CurrentRadiationSourceCount>
	{
	public:
		virtual ~CurrentRadiationSourceCountHandler() = default;

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::CurrentRadiationSourceCount& a_event, RE::BSTEventSource<RE::CurrentRadiationSourceCount>*) override
		{
			RE::BSAutoLock{ dataLock };
			if (auto radiationSourceCount = Forms::RadiationSourceCount_DO->GetForm<RE::ActorValueInfo>(); radiationSourceCount)
			{
				auto value = static_cast<float>(a_event.optionalValue.value_or(0));
				RE::PlayerCharacter::GetSingleton()
					->SetBaseActorValue(*radiationSourceCount, value);
			}

			eventDataStruct.eventReceived = true;
			return RE::BSEventNotifyControl::kContinue;
		}
	};

	class PipboyLightEventHandler :
		public RE::BSTValueEventSink<RE::PipboyLightEvent>
	{
	public:
		virtual ~PipboyLightEventHandler() = default;

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
		{
			RE::BSAutoLock{ dataLock };
			if (auto pipboyLightActive = Forms::PipboyLightActive_DO->GetForm<RE::ActorValueInfo>(); pipboyLightActive)
			{
				auto value = a_event.optionalValue.value_or(false) ? 1.0f : 0.0f;
				RE::PlayerCharacter::GetSingleton()
					->SetBaseActorValue(*pipboyLightActive, value);
			}

			eventDataStruct.eventReceived = true;
			return RE::BSEventNotifyControl::kContinue;
		}
	};

	class TESContainerChangedEventHandler :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{
	public:
		virtual ~TESContainerChangedEventHandler() = default;

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent& a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override
		{
			auto source = RE::TESForm::GetFormByID(a_event.oldContainerFormID);
			if (auto refr = source ? source->As<RE::TESObjectREFR>() : nullptr; refr)
			{
				F4SE::GetTaskInterface()->AddTask(new Tasks::WeightUpdate(refr));
			}

			auto target = RE::TESForm::GetFormByID(a_event.newContainerFormID);
			if (auto refr = target ? target->As<RE::TESObjectREFR>() : nullptr; refr)
			{
				F4SE::GetTaskInterface()->AddTask(new Tasks::WeightUpdate(refr));
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};

	// Used to register some globals that we can't register at game start time
	class MenuOpenCloseEventHandler :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual ~MenuOpenCloseEventHandler() = default;

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
		{
			if (a_event.opening && a_event.menuName == "HUDMenu")
			{
				logger::debug("Registering late event sinks.");
				RE::CurrentRadiationSourceCount::GetEventSource()->RegisterSink(new CurrentRadiationSourceCountHandler());
				RE::PipboyLightEvent::GetEventSource()->RegisterSink(new PipboyLightEventHandler());
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};

	void Register()
	{
		logger::debug("Registering EventHandlers.");
		RE::TESContainerChangedEvent::GetEventSource()->RegisterSink(new TESContainerChangedEventHandler());
		RE::UI::GetSingleton()->BSTEventSource<RE::MenuOpenCloseEvent>::RegisterSink(new MenuOpenCloseEventHandler());
	}
}