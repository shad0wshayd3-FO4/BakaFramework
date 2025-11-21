#pragma once

namespace ObScript::Help
{
	namespace FORM
	{
		namespace CELL
		{
			void Build();
		}

		void Print(const std::string_view& a_name, RE::ENUM_FORM_ID a_type);
	}

	namespace FUNC
	{
		void Print(const std::string_view& a_name);
	}

	namespace GLOB
	{
		void Print(const std::string_view& a_name);
	}

	namespace GMST
	{
		void Print(const std::string_view& a_name);
	}

	namespace HELP
	{
		void Print();
	}

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
		char a_type[512]{ '\0' };

		std::uint32_t a_code{ 0 };
		RE::Script::ParseParameters(
			a_parameters,
			a_compiledParams,
			a_offset,
			a_refObject,
			a_container,
			a_script,
			a_scriptLocals,
			a_name,
			&a_code,
			a_type);

		std::string_view name = a_name;
		const auto       type = RE::TESForm::GetFormTypeFromString(a_type);

		if (name.empty())
		{
			HELP::Print();
			return true;
		}

		switch (a_code)
		{
		case 0:
			FUNC::Print(name);
			GMST::Print(name);
			GLOB::Print(name);
			FORM::Print(name, type);
			HELP::Print();
			break;
		case 1:
			FUNC::Print(name);
			HELP::Print();
			break;
		case 2:
			GMST::Print(name);
			HELP::Print();
			break;
		case 3:
			GLOB::Print(name);
			HELP::Print();
			break;
		case 4:
			FORM::Print(name, type);
			HELP::Print();
			break;
		default:
			HELP::Print();
			break;
		}

		return true;
	}

	inline void Install()
	{
		if (auto func = RE::SCRIPT_FUNCTION::LocateConsoleCommand("Help"sv))
		{
			static RE::SCRIPT_PARAMETER params[] = {
				{ "matchstring (optional)", RE::SCRIPT_PARAM_TYPE::kChar, true },
				{ "filter (optional)",      RE::SCRIPT_PARAM_TYPE::kInt,  true },
				{ "form type (optional)",   RE::SCRIPT_PARAM_TYPE::kChar, true }
			};

			func->SetParameters(params);
			func->executeFunction = &Execute;
		}
	}
}
