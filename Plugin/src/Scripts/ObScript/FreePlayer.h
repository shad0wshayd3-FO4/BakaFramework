#pragma once

namespace ObScript
{
	class FreePlayer
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
					return _stricmp(a_elem.functionName, "LuaConsole") == 0;
				});

			if (it != functions.end())
			{
				*it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
				it->helpString = HelpString().data();
				it->executeFunction = Execute;

				DEBUG("Registered FreePlayer."sv);
			}
			else
			{
				DEBUG("Failed to register FreePlayer."sv);
			}
		}

	private:
		static bool Execute(
			const RE::SCRIPT_PARAMETER*,
			const char*,
			RE::TESObjectREFR*,
			RE::TESObjectREFR*,
			RE::Script*,
			RE::ScriptLocals*,
			float&,
			std::uint32_t&)
		{
			RE::MemoryManager::GetSingleton().Deallocate(RE::PlayerCharacter::GetSingleton(), false);
			return true;
		}

		[[nodiscard]] static const std::string& HelpString()
		{
			static auto help = []()
			{
				std::string buf;
				buf += ""sv;
				return buf;
			}();
			return help;
		}

		static constexpr auto LONG_NAME = "FreePlayer"sv;
		static constexpr auto SHORT_NAME = "GoodbyeWorldspace"sv;
	};
}
