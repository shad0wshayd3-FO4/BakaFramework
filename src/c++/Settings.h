#pragma once

namespace Settings
{
	namespace
	{
		using ISetting = AutoTOML::ISetting;
		using bSetting = AutoTOML::bSetting;
		using sSetting = AutoTOML::sSetting;
	}

	namespace General
	{
		inline bSetting EnableDebugLogging{ "General"s, "EnableDebugLogging"s, false };
	}

	namespace Config
	{
		inline sSetting BetaCommentFileName{ "Config"s, "BetaCommentFileName"s, "BetaComment.txt"s };
	}

	namespace Patches
	{
		inline bSetting EnableLoadingEditorIDs{ "Patches"s, "EnableLoadingEditorIDs"s, true, true };
		inline bSetting EnableEDIDConflictCheck{ "Patches"s, "EnableEditorIDConflictCheck"s, false, true };
	}

	namespace Features
	{
		inline bSetting EnablePAStorage{ "Features"s, "EnablePowerArmorStorage"s, true, true };
	}

	inline void Load()
	{
		try
		{
			const auto table = toml::parse_file(fmt::format(FMT_STRING("Data/F4SE/Plugins/{}.toml"sv), Plugin::NAME));
			for (const auto& setting : ISetting::get_settings())
			{
				setting->load(table);
			}
		}
		catch (const toml::parse_error& e)
		{
			std::ostringstream ss;
			ss
				<< "Error parsing file \'" << *e.source().path << "\':\n"
				<< '\t' << e.description() << '\n'
				<< "\t\t(" << e.source().begin << ')';
			logger::error(FMT_STRING("{}"sv), ss.str());
			stl::report_and_fail("Failed to load settings."sv);
		}
		catch (const std::exception& e)
		{
			stl::report_and_fail(e.what());
		}
		catch (...)
		{
			stl::report_and_fail("Unknown failure."sv);
		}
	}
}
