#pragma once

namespace ObScript
{
	class QuitGame
	{
	public:
		static void Install()
		{
			const auto functions = RE::SCRIPT_FUNCTION::GetConsoleFunctions();
			const auto it = std::find_if(
				functions.begin(),
				functions.end(),
				[&](auto&& a_elem)
				{
					return _stricmp(a_elem.functionName, "QuitGame") == 0;
				});

			if (it != functions.end())
			{
				*it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
				it->helpString = HelpString().data();
				it->executeFunction = Execute;

				logger::debug("Registered QuitGame."sv);
			}
			else
			{
				logger::debug("Failed to register QuitGame."sv);
			}
		}

	private:
		static bool Execute(const RE::SCRIPT_PARAMETER*, const char*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, float&, std::uint32_t&)
		{
			RE::ConsoleLog::GetSingleton()->AddString("Bye.\n");
			std::thread([&]()
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(50));
							RE::Main::GetSingleton()->quitGame = true;
						})
				.detach();

			return true;
		}

		[[nodiscard]] static const std::string& HelpString()
		{
			static auto help = []()
			{
				std::string buf;
				buf += "Exit game without going through menus.";
				return buf;
			}();
			return help;
		}

		static constexpr auto LONG_NAME = "QuitGame"sv;
		static constexpr auto SHORT_NAME = "qqq"sv;
	};
}
