#include "Config/Config.h"
#include "Events/Events.h"
#include "Forms/Forms.h"
#include "Misc/Patches.h"
#include "Scripts/ObScript.h"
#include "Scripts/Papyrus.h"

namespace Serialization
{
	namespace
	{
		void RevertCallback([[maybe_unused]] const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::PipboyLightEventHandler::GetSingleton()->Revert();
		}

		void SaveCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::PipboyLightEventHandler::GetSingleton()->Save(a_intfc);
		}

		void LoadCallback(const F4SE::SerializationInterface* a_intfc)
		{
			Papyrus::BakaUtil::PipboyLightEventHandler::GetSingleton()->Load(a_intfc);
		}
	}

	void Register()
	{
		const auto serialization = F4SE::GetSerializationInterface();
		if (!serialization)
		{
			REX::WARN("Failed to register Serialization callbacks."sv);
		}

		serialization->SetUniqueID(static_cast<std::uint32_t>('BFRM'));
		serialization->SetRevertCallback(RevertCallback);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);
	}
}

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
		{
			Config::Load();
			Forms::Install();
			Patches::Install();
			ObScript::Install();
			break;
		}
		case F4SE::MessagingInterface::kGameDataReady:
		{
			if (static_cast<bool>(a_msg->data))
			{
				Events::Register();
				ObScript::Help::FORM::CELL::Build();
			}

			break;
		}

		default:
			break;
		}
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	Serialization::Register();

	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
