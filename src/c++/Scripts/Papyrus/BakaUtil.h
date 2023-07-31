#pragma once

#include "SteamAPI/SteamAPI.h"

//#include "Workshop/Workshop.h"

namespace Papyrus::BakaUtil
{
	namespace detail
	{
		class PipboyLightEventMap
		{
		public:
			static void Register(const RE::BSScript::Variable* a_variable)
			{
				if (!a_variable->is<RE::BSScript::Object>())
				{
					return;
				}

				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
					if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
					{
						return;
					}

					if (!object->type)
					{
						return;
					}

					const RE::BSAutoLock lock{ spin };
					GameVM->handlePolicy.PersistHandle(object->GetHandle());
					map.emplace(
						object->GetHandle(),
						object->type->GetName());
				}
			}

			static void Unregister(const RE::BSScript::Variable* a_variable)
			{
				if (!a_variable->is<RE::BSScript::Object>())
				{
					return;
				}

				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
					if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
					{
						return;
					}

					const RE::BSAutoLock lock{ spin };
					GameVM->handlePolicy.ReleaseHandle(object->GetHandle());
					map.erase(object->GetHandle());
				}
			}

			static void Dispatch(bool a_value)
			{
				auto GameVM = RE::GameVM::GetSingleton();
				if (!GameVM)
				{
					return;
				}

				auto VM = GameVM->GetVM();
				if (!VM)
				{
					return;
				}

				const RE::BSAutoLock lock{ spin };
				for (auto& [handle, string] : map)
				{
					VM->DispatchMethodCall(
						handle,
						string,
						"OnPipboyLightEvent"sv,
						nullptr,
						a_value);
				}
			}

			static void Revert([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
			{
				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					const RE::BSAutoLock lock{ spin };
					for (auto& [handle, string] : map)
					{
						GameVM->handlePolicy.ReleaseHandle(handle);
					}

					map.clear();
				}
			}

			static void Save([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
			{
				const RE::BSAutoLock lock{ spin };
				a_intfc->OpenRecord('PLEH', 2);

				std::uint32_t size{ map.size() };
				a_intfc->WriteRecordData(&size, sizeof(size));

				for (auto& [handle, string] : map)
				{
					a_intfc->WriteRecordData(&handle, sizeof(handle));
					std::uint32_t length{ string.size() };
					a_intfc->WriteRecordData(&length, sizeof(length));
					a_intfc->WriteRecordData(string.data(), length);
				}
			}

			static void Load([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
			{
				Revert(a_intfc);

				const RE::BSAutoLock lock{ spin };
				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					std::uint32_t sig{ 0 }, ver{ 0 }, len{ 0 };
					while (a_intfc->GetNextRecordInfo(sig, ver, len))
					{
						if (sig != 'PLEH')
						{
							continue;
						}

						if (ver != 2)
						{
							continue;
						}

						std::uint32_t size{ 0 };
						a_intfc->ReadRecordData(&size, sizeof(size));

						for (std::uint32_t i = 0; i < size; i++)
						{
							std::uint64_t handle{ 0 };
							a_intfc->ReadRecordData(&handle, sizeof(handle));

							auto resolve = a_intfc->ResolveHandle(handle);
							if (!resolve)
							{
								continue;
							}

							std::uint32_t length{ 0 };
							a_intfc->ReadRecordData(&length, sizeof(length));

							std::string string;
							string.reserve(length);
							a_intfc->ReadRecordData(string.data(), length);

							handle = resolve.value();
							GameVM->handlePolicy.PersistHandle(handle);
							map.emplace(
								handle,
								string.c_str());
						}

						return;
					}
				}
			}

		private:
			inline static RE::BSSpinLock spin;
			inline static RE::BSTHashMap<std::uint64_t, RE::BSFixedString> map;
		};

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
				PipboyLightEventMap::Dispatch(a_event.optionalValue.value_or(false));

				eventDataStruct.eventReceived = true;
				return RE::BSEventNotifyControl::kContinue;
			}
		};
	}

	constexpr auto SCRIPT_NAME{ "BakaUtil"sv };

	/*
	bool CreatePowerArmorToken(std::monostate, RE::TESObjectREFR* a_refr)
	{
		return Workshop::PlacementMode::CreateToken(a_refr);
	}

	bool HandlePowerArmorToken(std::monostate, RE::TESObjectREFR* a_refr)
	{
		return Workshop::PlacementMode::HandleToken(a_refr);
	}
	*/

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

	void OpenWebPage(std::monostate, std::string_view a_url, bool a_fallback)
	{
		SteamAPI::OpenWebPage(a_url.data(), a_fallback);
	}

	void RegisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventMap::Register(a_this);
	}

	void UnregisterForPipboyLightEvent(std::monostate, const RE::BSScript::Variable* a_this)
	{
		detail::PipboyLightEventMap::Unregister(a_this);
	}
}
