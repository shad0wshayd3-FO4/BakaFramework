#pragma once

#include "Scripts/Papyrus/BakaUtil.h"

namespace Serialization
{
	namespace
	{
		void RevertCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventHandler::GetSingleton()->Revert();
		}

		void SaveCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventHandler::GetSingleton()->Save(a_intfc);
		}

		void LoadCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventHandler::GetSingleton()->Load(a_intfc);
		}
	}

	void Register()
	{
		const auto serialization = F4SE::GetSerializationInterface();
		if (!serialization)
		{
			REX::CRITICAL("Failed to register Serialization callbacks, marking as incompatible."sv);
		}

		serialization->SetUniqueID(static_cast<std::uint32_t>('BFRM'));
		serialization->SetRevertCallback(RevertCallback);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);
	}
}
