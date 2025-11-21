#pragma once

namespace ObScript::GetDefaultObject
{
	static bool Execute(
		[[maybe_unused]] const RE::SCRIPT_PARAMETER* a_parameters,
		[[maybe_unused]] const char*                 a_compiledParams,
		[[maybe_unused]] RE::TESObjectREFR*          a_refObject,
		[[maybe_unused]] RE::TESObjectREFR*          a_container,
		[[maybe_unused]] RE::Script*                 a_script,
		[[maybe_unused]] RE::ScriptLocals*           a_scriptLocals,
		[[maybe_unused]] float&                      a_result,
		[[maybe_unused]] std::uint32_t&              a_offset)
	{
		char a_name[512]{ '\0' };
		RE::Script::ParseParameters(
			a_parameters,
			a_compiledParams,
			a_offset,
			a_refObject,
			a_container,
			a_script,
			a_scriptLocals,
			a_name);

		std::string_view name = a_name;
		if (name.empty())
		{
			return true;
		}

		if (auto form = RE::TESForm::GetFormByEditorID(a_name))
		{
			if (auto defaultObject = form->As<RE::BGSDefaultObject>())
			{
				RE::ConsoleLog::GetSingleton()->Log("GetDefaultObject ({}) >> {:08X}"sv,
					defaultObject->GetFormEditorID(), defaultObject->form ? defaultObject->form->GetFormID() : 0);
				return true;
			}
		}

		RE::ConsoleLog::GetSingleton()->Log("DefaultObject \"{}\" Does Not Exist"sv, a_name);
		return true;
	}

	inline void Install()
	{
		if (auto func = RE::SCRIPT_FUNCTION::LocateConsoleCommand("DumpSoundMap"sv))
		{
			static RE::SCRIPT_PARAMETER params[] = {
				{ "String", RE::SCRIPT_PARAM_TYPE::kChar, false },
			};

			func->functionName = "GetDefaultObject";
			func->shortName = "gdo";
			func->helpString = "Print the FormID contained by a DefaultObject form. [GetDefaultObject \"PowerArmorDefaultBatteryObject_DO\"]";
			func->referenceFunction = false;
			func->SetParameters(params);
			func->executeFunction = &Execute;
		}
	}
}
