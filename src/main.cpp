#include "Events/Events.h"
#include "Forms/Forms.h"
#include "Scripts/Scripts.h"
#include "Patches/Patches.h"
#include "Serialization/Serialization.h"
#include "Settings/Settings.h"

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
			ObScript::Install();
			Patches::Install();
			Settings::Load();
			break;
		case F4SE::MessagingInterface::kGameDataReady:
			Events::Register();
			Forms::Install();
			ObScript::Help::FORM::CELL::Build();
			break;
		default:
			break;
		}
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);
	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	if (const auto intfc = F4SE::GetSerializationInterface())
	{
		intfc->SetUniqueID(static_cast<std::uint32_t>('BFRM'));
		intfc->SetSaveCallback(Serialization::SaveCallback);
		intfc->SetLoadCallback(Serialization::LoadCallback);
		intfc->SetRevertCallback(Serialization::RevertCallback);
		intfc->SetFormDeleteCallback(Serialization::FormDeleteCallback);
	}

	return true;
}
