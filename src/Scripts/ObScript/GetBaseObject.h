#pragma once

namespace ObScript::GetBaseObject
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
		if (!a_refObject || !a_refObject->data.objectReference)
		{
			return true;
		}

		RE::ConsoleLog::GetSingleton()->Log("GetBaseObject >> {:08X}"sv,
			a_refObject->data.objectReference->formID);
		return true;
	}

	inline void Install()
	{
		if (auto func = RE::SCRIPT_FUNCTION::LocateConsoleCommand("IgnoreRenderPass"sv))
		{
			func->functionName = "GetBaseObject";
			func->shortName = "gbo";
			func->helpString = "Print the FormID of an ObjectReference's base form.";
			func->referenceFunction = true;
			func->SetParameters();
			func->executeFunction = &Execute;
		}
	}
}
