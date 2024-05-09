#include "Events/Events.h"
#include "Forms/Forms.h"
#include "Misc/Patches.h"
#include "Scripts/ObScript.h"
#include "Scripts/Papyrus.h"
#include "Serialization/Serialization.h"

namespace
{
	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg)
		{
			return;
		}

		switch (a_msg->type)
		{
			case F4SE::MessagingInterface::kPostLoad:
				{
					Forms::Install();
					break;
				}
			case F4SE::MessagingInterface::kGameDataReady:
				{
					if (static_cast<bool>(a_msg->data))
					{
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

DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", Plugin::NAME.data(), NULL);
#endif

	Config::Load();

	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(1u << 10);

	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	INFO("{} v{} loaded."sv, Plugin::NAME, Plugin::Version);

	Serialization::Register();

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		FATAL("Failed to register messaging handler, marking as incompatible."sv);
		return false;
	}

	const auto papyrus = F4SE::GetPapyrusInterface();
	if (!papyrus || !papyrus->Register(Papyrus::RegisterFunctions))
	{
		FATAL("Failed to register Papyrus functions, marking as incompatible."sv);
		return false;
	}

	Patches::Install();
	ObScript::Install();

	INFO("Plugin loaded successfully."sv);

	return true;
}
