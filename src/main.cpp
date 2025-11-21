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

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .trampoline = true, .trampolineSize = 1024 });

	Serialization::Register();

	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
