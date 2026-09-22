#pragma once

#include "Serialization/Serialization.h"

namespace Papyrus::BakaEvents
{
	namespace ActiveMagicEffect
	{
		void RegisterForPipboyLight(std::monostate, RE::ActiveEffect* a_activeEffect)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Register(a_activeEffect);
		}

		void UnregisterForPipboyLight(std::monostate, RE::ActiveEffect* a_activeEffect)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Unregister(a_activeEffect);
		}

		void RegisterForRadiationCount(std::monostate, RE::ActiveEffect* a_activeEffect)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Register(a_activeEffect);
		}

		void UnregisterForRadiationCount(std::monostate, RE::ActiveEffect* a_activeEffect)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Unregister(a_activeEffect);
		}
	}

	namespace Alias
	{
		void RegisterForPipboyLight(std::monostate, RE::BGSBaseAlias* a_alias)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Register(a_alias);
		}

		void UnregisterForPipboyLight(std::monostate, RE::BGSBaseAlias* a_alias)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Unregister(a_alias);
		}

		void RegisterForRadiationCount(std::monostate, RE::BGSBaseAlias* a_alias)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Register(a_alias);
		}

		void UnregisterForRadiationCount(std::monostate, RE::BGSBaseAlias* a_alias)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Unregister(a_alias);
		}
	}

	namespace Form
	{
		void RegisterForPipboyLight(std::monostate, RE::TESForm* a_form)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Register(a_form);
		}

		void UnregisterForPipboyLight(std::monostate, RE::TESForm* a_form)
		{
			Serialization::EventManager::GetSingleton()->pipboyLight.Unregister(a_form);
		}

		void RegisterForRadiationCount(std::monostate, RE::TESForm* a_form)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Register(a_form);
		}

		void UnregisterForRadiationCount(std::monostate, RE::TESForm* a_form)
		{
			Serialization::EventManager::GetSingleton()->radiationCount.Unregister(a_form);
		}
	}

	static void RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm)
	{
		// ActiveMagicEffect
		a_vm->BindNativeMethod("BakaEvents_ActiveMagicEffect"sv, "RegisterForPipboyLight"sv, ActiveMagicEffect::RegisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_ActiveMagicEffect"sv, "UnregisterForPipboyLight"sv, ActiveMagicEffect::UnregisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_ActiveMagicEffect"sv, "RegisterForRadiationCount"sv, ActiveMagicEffect::RegisterForRadiationCount, true);
		a_vm->BindNativeMethod("BakaEvents_ActiveMagicEffect"sv, "UnregisterForRadiationCount"sv, ActiveMagicEffect::UnregisterForRadiationCount, true);

		// Alias
		a_vm->BindNativeMethod("BakaEvents_Alias"sv, "RegisterForPipboyLight"sv, Alias::RegisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_Alias"sv, "UnregisterForPipboyLight"sv, Alias::UnregisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_Alias"sv, "RegisterForRadiationCount"sv, Alias::RegisterForRadiationCount, true);
		a_vm->BindNativeMethod("BakaEvents_Alias"sv, "UnregisterForRadiationCount"sv, Alias::UnregisterForRadiationCount, true);

		// Form
		a_vm->BindNativeMethod("BakaEvents_Form"sv, "RegisterForPipboyLight"sv, Form::RegisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_Form"sv, "UnregisterForPipboyLight"sv, Form::UnregisterForPipboyLight, true);
		a_vm->BindNativeMethod("BakaEvents_Form"sv, "RegisterForRadiationCount"sv, Form::RegisterForRadiationCount, true);
		a_vm->BindNativeMethod("BakaEvents_Form"sv, "UnregisterForRadiationCount"sv, Form::UnregisterForRadiationCount, true);
	}
}
