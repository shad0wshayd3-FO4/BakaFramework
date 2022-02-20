#pragma once

#include "Forms/Forms.h"

namespace Events::Handlers
{
	namespace
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
				if (auto refr = m_refr.get())
				{
					if (auto inventoryWeight = Forms::InventoryWeight_DO->GetForm<RE::ActorValueInfo>())
					{
						refr->SetBaseActorValue(*inventoryWeight, refr->GetWeightInContainer());
					}
				}
			}

		protected:
			RE::BSTSmartPointer<RE::TESObjectREFR, RE::BSTSmartPointerGamebryoRefCount> m_refr;
		};
	}

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
			auto source = RE::TESForm::GetFormByID(a_event.oldContainerFormID);
			if (auto refr = source ? source->As<RE::TESObjectREFR>() : nullptr)
			{
				F4SE::GetTaskInterface()->AddTask(new WeightUpdate(refr));
			}

			auto target = RE::TESForm::GetFormByID(a_event.newContainerFormID);
			if (auto refr = target ? target->As<RE::TESObjectREFR>() : nullptr)
			{
				F4SE::GetTaskInterface()->AddTask(new WeightUpdate(refr));
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};
}
