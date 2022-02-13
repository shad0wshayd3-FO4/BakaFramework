#pragma once

#include "Shared/FunctionArgs.h"

namespace Papyrus::Actor
{
	namespace detail
	{
		class FilterPlaceholder
		{
		public:
			std::byte space[0x18];	// 00
			void* impl{ nullptr };	// 18
		};
		static_assert(sizeof(FilterPlaceholder) == 0x20);

		void SendEventToObjectAndRelated(
			RE::GameVM* a_this,
			std::size_t a_object,
			const RE::BSFixedString& a_eventName,
			const RE::BSTThreadScrapFunction<bool(RE::BSScrapArray<RE::BSScript::Variable>&)>& a_args,
			const FilterPlaceholder& a_filter = FilterPlaceholder(),
			const RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>& a_callback = nullptr)
		{
			using func_t = decltype(&detail::SendEventToObjectAndRelated);
			REL::Relocation<func_t> func{ REL::ID(367992) };
			return func(a_this, a_object, a_eventName, a_args, a_filter, a_callback);
		}
	}

	inline RE::BSEventNotifyControl hkProcessEvent(void*, const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		auto GameVM = RE::GameVM::GetSingleton();
		if (!GameVM)
		{
			return RE::BSEventNotifyControl::kContinue;
		}

		if (auto victim = a_event.actorDying.get())
		{
			auto handle = GameVM->handlePolicy.GetHandleForObject(
				static_cast<std::uint32_t>(victim->GetFormType()),
				victim);
			auto args = FunctionArgs{ GameVM->GetVM().get(), victim };
			detail::SendEventToObjectAndRelated(
				GameVM,
				handle,
				a_event.dead ? "OnDeath"sv : "OnDying"sv,
				args.CreateThreadScrapFunction());
		}

		if (auto killer = a_event.actorKiller.get())
		{
			auto handle = GameVM->handlePolicy.GetHandleForObject(
				static_cast<std::uint32_t>(killer->GetFormType()),
				killer);
			auto args = FunctionArgs{ GameVM->GetVM().get(), killer };
			detail::SendEventToObjectAndRelated(
				GameVM,
				handle,
				a_event.dead ? "OnKill"sv : "OnKilling"sv,
				args.CreateThreadScrapFunction());
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> targetVtbl{ RE::GameScript::BasicEventHandler::VTABLE[25] };
		targetVtbl.write_vfunc(0x01, hkProcessEvent);
	}
}
