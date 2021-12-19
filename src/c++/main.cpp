#include "Events/Events.h"
#include "Forms/Forms.h"
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
					logger::debug("PostLoad"sv);
					break;
				}

			case F4SE::MessagingInterface::kPostPostLoad:
				{
					logger::debug("PostPostLoad"sv);
					break;
				}

			case F4SE::MessagingInterface::kGameLoaded:
				{
					logger::debug("GameLoaded"sv);
					break;
				}

			case F4SE::MessagingInterface::kGameDataReady:
				{
					if (static_cast<bool>(a_msg->data))
					{
						logger::debug("GameDataReady - Loaded"sv);
						Events::Register();
					}
					else
					{
						logger::debug("GameDataReady - Unloaded"sv);
					}

					break;
				}

			default:
				break;
		}
	}

	void InitializeLog()
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path)
		{
			stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format(FMT_STRING("{}.log"), Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level =
			*Settings::General::EnableDebugLogging ? spdlog::level::trace : spdlog::level::info;
#endif

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%m/%d/%Y - %T] [%^%l%$] %v"s);
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_F4SE, F4SE::PluginInfo* a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION[0];

	const auto rtv = a_F4SE->RuntimeVersion();
	if (rtv < F4SE::RUNTIME_LATEST)
	{
		stl::report_and_fail(
			fmt::format(FMT_STRING("{} does not support runtime v{}."sv), Plugin::NAME, rtv.string()));
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
	Settings::Load();

	InitializeLog();
	logger::info(FMT_STRING("{} v{} log opened."sv), Plugin::NAME, Plugin::VERSION.string());
	logger::debug("Debug logging enabled."sv);

	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(1 << 5);

	Serialization::Register();

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		logger::critical("Failed to get F4SE messaging interface, marking as incompatible."sv);
		return false;
	}

	const auto papyrus = F4SE::GetPapyrusInterface();
	if (!papyrus || !papyrus->Register(Papyrus::RegisterFunctions))
	{
		logger::critical("Failed to register Papyrus functions, marking as incompatible."sv);
		return false;
	}

	Forms::InstallHooks();
	ObScript::Install();

	logger::info("Plugin loaded successfully."sv);

	return true;
}
