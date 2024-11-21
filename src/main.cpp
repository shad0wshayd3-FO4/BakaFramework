#include "Config/Config.h"
#include "Events/Events.h"
#include "Forms/Forms.h"
#include "Misc/Patches.h"
#include "Scripts/ObScript.h"
#include "Scripts/Papyrus.h"
#include "Serialization/Serialization.h"

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
		{
			Forms::Install();
			Patches::Install();
			ObScript::Install();
			break;
		}
		case F4SE::MessagingInterface::kGameDataReady:
		{
			if (static_cast<bool>(a_msg->data))
			{
				Config::Load();
				Events::Register();
			}
			else
			{
				ObScript::Help::ClearCellMap();
			}

			break;
		}

		default:
			break;
		}
	}
}

F4SEPluginLoad(const F4SE::LoadInterface* a_F4SE)
{
	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(1u << 10);

	Serialization::Register();

	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
