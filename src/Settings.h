#pragma once

class Settings
{
public:
	using ISetting = AutoTOML::ISetting;
	using bSetting = AutoTOML::bSetting;
	using sSetting = AutoTOML::sSetting;

	static void Load()
	{
		try
		{
			const auto table = toml::parse_file("Data/F4SE/Plugins/BakaFramework.toml"s);
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
			logger::error(ss.str());
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

	static inline bSetting EnableDebugLogging{ "General"s, "EnableDebugLogging"s, false };
	static inline sSetting BetaCommentFileName{ "Config"s, "BetaCommentFileName"s, "BetaComment.txt"s };

private:
	Settings() = delete;
	Settings(const Settings&) = delete;
	Settings(Settings&&) = delete;

	~Settings() = delete;

	Settings& operator=(const Settings&) = delete;
	Settings& operator=(Settings&&) = delete;
};