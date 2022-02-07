#pragma once

#include "Workshop/Workshop.h"

namespace ObScript
{
	class IsWorkshopPlacementActive
	{
	public:
		static void Install()
		{
			const auto functions = RE::SCRIPT_FUNCTION::GetScriptFunctions();
			const auto it = std::find_if(
				functions.begin(),
				functions.end(),
				[&](auto&& a_elem)
				{
					return _stricmp(a_elem.functionName, "IsHorseStolen") == 0;
				});

			if (it != functions.end())
			{
				*it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
				it->helpString = HelpString().data();
				it->referenceFunction = false;
				it->executeFunction = Execute;
				it->conditionFunction = Condition;

				logger::debug("Registered IsWorkshopPlacementModeActive."sv);
			}
			else
			{
				logger::debug("Failed to register IsWorkshopPlacementModeActive."sv);
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
			auto result =
				fmt::format(
					FMT_STRING("IsWorkshopPlacementModeActive >> {:d}\n"sv),
					Workshop::PlacementMode::IsActive());
			RE::ConsoleLog::GetSingleton()->AddString(result.c_str());
			return true;
		}

		static bool Condition(
			RE::ConditionCheckParams&,
			void*,
			void*,
			float& a_returnValue)
		{
			a_returnValue = Workshop::PlacementMode::IsActive() ? 1.0f : 0.0f;
			return true;
		}

		[[nodiscard]] static const std::string& HelpString()
		{
			static auto help = []()
			{
				std::string buf;
				buf += "Returns true if BakaFramework's Workshop Placement mode is active.";
				return buf;
			}();
			return help;
		}

		static constexpr auto LONG_NAME = "IsWorkshopPlacementModeActive"sv;
		static constexpr auto SHORT_NAME = "wpa"sv;
	};
}
