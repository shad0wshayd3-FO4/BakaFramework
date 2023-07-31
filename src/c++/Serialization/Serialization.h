#pragma once

namespace Serialization
{
	namespace
	{
		void RevertCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
		}

		void SaveCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
		}

		void LoadCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
		}
	}

	bool Register()
	{
		const auto serialization = F4SE::GetSerializationInterface();
		if (!serialization)
		{
			logger::critical("Failed to register Serialization callbacks, marking as incompatible."sv);
			return false;
		}

		serialization->SetUniqueID(static_cast<std::uint32_t>('BFRM'));
		serialization->SetRevertCallback(RevertCallback);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);

		return true;
	}
}
