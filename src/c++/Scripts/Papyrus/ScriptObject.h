#pragma once

#include "Shared/RegistrationMap.h"

namespace Papyrus::ScriptObject
{
	constexpr auto SCRIPT_NAME{ "ScriptObject"sv };

	class PipboyLightEventHandler :
		public RE::BSTValueEventSink<RE::PipboyLightEvent>,
		public RegistrationMap<bool>
	{
	public:
		[[nodiscard]] static PipboyLightEventHandler* GetSingleton()
		{
			static PipboyLightEventHandler singleton;
			return std::addressof(singleton);
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::PipboyLightEvent& a_event, RE::BSTEventSource<RE::PipboyLightEvent>*) override
		{
			auto value = a_event.optionalValue.value_or(false);
			if (value != valueCache)
			{
				DispatchEvent("OnPipboyLightEvent"sv, value);
				valueCache = value;
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		virtual constexpr std::uint32_t GetRecordVersion() override { return 1; }
		virtual constexpr std::uint32_t GetRecordType() override { return 'PLEH'; }
		virtual constexpr std::string_view GetRecordName() override { return "PLEH"sv; }

		virtual bool Save(const F4SE::SerializationInterface* a_intfc) override
		{
			auto result = RegistrationMap<bool>::Save(a_intfc);
			if (result)
			{
				if (!a_intfc->OpenRecord('PLEC', 1))
				{
					logger::warn(FMT_STRING("{:s}: Failed to create record."), "PLEC"sv);
					return false;
				}

				if (!a_intfc->WriteRecordData(&valueCache, sizeof(valueCache)))
				{
					logger::warn(FMT_STRING("{:s}: Failed to write record data."), "PLEC"sv);
					return false;
				}

				logger::info(FMT_STRING("{:s}: Saved."), "PLEC"sv);
			}

			return result;
		}

		virtual bool Load(const F4SE::SerializationInterface* a_intfc) override
		{
			std::uint32_t type{ 0 }, version{ 0 }, length{ 0 };
			while (a_intfc->GetNextRecordInfo(type, version, length))
			{
				switch (type)
				{
					case 'PLEH':
						{
							if (!LoadImpl(a_intfc))
							{
								return false;
							}
						}
						break;

					case 'PLEC':
						{
							if (sizeof(valueCache) != a_intfc->ReadRecordData(&valueCache, sizeof(valueCache)))
							{
								logger::warn(FMT_STRING("{:s}: Failed to read record data."), "PLEC"sv);
								return false;
							}

							logger::info(FMT_STRING("{:s}: Loaded."), "PLEC"sv);
						}
						break;

					default:
						break;
				}
			}

			return true;
		}

		virtual void Clear() override
		{
			RegistrationMap<bool>::Clear();
			valueCache = false;
		}

	protected:
		bool valueCache{ false };
	};

	void RegisterForPipboyLightEvent(RE::BSScript::Object& a_this)
	{
		PipboyLightEventHandler::GetSingleton()->Register(a_this);
	}

	void UnregisterForPipboyLightEvent(RE::BSScript::Object& a_this)
	{
		PipboyLightEventHandler::GetSingleton()->Unregister(a_this);
	}
}
