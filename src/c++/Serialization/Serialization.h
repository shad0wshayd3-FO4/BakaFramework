#pragma once

#include "Scripts/Papyrus/BakaUtil.h"

namespace Serialization
{
	namespace
	{
		void RevertCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventMap::Revert(a_intfc);
		}

		void SaveCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventMap::Save(a_intfc);
		}

		void LoadCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::detail::PipboyLightEventMap::Load(a_intfc);
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
