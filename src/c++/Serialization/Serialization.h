#pragma once

#include "Scripts/Papyrus/ScriptObject.h"

namespace Serialization
{
	namespace
	{
		void RevertCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::ScriptObject::PipboyLightEventHandler::GetSingleton()->Clear();
		}

		void SaveCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::ScriptObject::PipboyLightEventHandler::GetSingleton()->Save(a_intfc);
		}

		void LoadCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::ScriptObject::PipboyLightEventHandler::GetSingleton()->Load(a_intfc);
		}
	}

	bool Register()
	{
		const auto serialization = F4SE::GetSerializationInterface();
		if (!serialization)
		{
			logger::critical("Failed to register Serialization callbacks, marking as incompatible.");
			return false;
		}

		serialization->SetUniqueID(static_cast<std::uint32_t>('BFRM'));
		serialization->SetRevertCallback(RevertCallback);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);

		return true;
	}
}
