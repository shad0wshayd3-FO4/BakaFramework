#pragma once

namespace ObScript
{
	class GetDefaultObject
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
					return _stricmp(a_elem.functionName, "DumpSoundMap") == 0;
				});

			if (it != functions.end())
			{
				static std::array params{
					RE::SCRIPT_PARAMETER{"String", RE::SCRIPT_PARAM_TYPE::kChar, false},
				};

				*it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
				it->helpString = HelpString().data();
				it->referenceFunction = false;
				it->paramCount = static_cast<std::uint16_t>(params.size());
				it->parameters = params.data();
				it->executeFunction = Execute;

				F4SE::log::debug("Registered GetDefaultObject."sv);
			}
			else
			{
				F4SE::log::debug("Failed to register GetDefaultObject."sv);
			}
		}

	private:
		static bool Execute(
			const RE::SCRIPT_PARAMETER* a_parameters,
			const char* a_compiledParams,
			RE::TESObjectREFR* a_refObject,
			RE::TESObjectREFR* a_container,
			RE::Script* a_script,
			RE::ScriptLocals* a_scriptLocals,
			float&,
			std::uint32_t& a_offset)
		{
			char dfobName[512]{ '\0' };
			RE::Script::ParseParameters(
				a_parameters,
				a_compiledParams,
				a_offset,
				a_refObject,
				a_container,
				a_script,
				a_scriptLocals,
				dfobName);

			if (dfobName[0] == '\0')
			{
				return true;
			}

			if (auto form = RE::TESForm::GetFormByEditorID(dfobName))
			{
				if (auto dfob = form->As<RE::BGSDefaultObject>(); dfob)
				{
					auto result = std::format(
						"GetDefaultObject ({:s}) >> 0x{:08X}"sv,
						dfob->formEditorID.c_str(),
						dfob->form ? dfob->form->formID : 0);
					RE::ConsoleLog::GetSingleton()->PrintLine(result.data());
					return true;
				}
			}

			auto result = std::format("GetDefaultObject ({:s}) >> Does not exist."sv, dfobName);
			RE::ConsoleLog::GetSingleton()->PrintLine(result.data());
			return true;
		}

		[[nodiscard]] static const std::string& HelpString()
		{
			static auto help = []()
			{
				std::string buf;
				buf += "Print the FormID contained by a DefaultObject Form. "sv;
				buf += "[GetDefaultObject \"InventoryWeight_DO\"]"sv;
				return buf;
			}();
			return help;
		}

		static constexpr auto LONG_NAME = "GetDefaultObject"sv;
		static constexpr auto SHORT_NAME = "gdo"sv;
	};
}
