#pragma once

namespace ObScript::SpellHasCastingPerk
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
		return false;
	}

	static bool Condition(
		[[maybe_unused]] RE::ConditionCheckParams& a_data,
		[[maybe_unused]] void*                     a_param1,
		[[maybe_unused]] void*                     a_param2,
		[[maybe_unused]] float&                    a_returnValue)
	{
		a_returnValue = 0.0f;
		if (a_data.actionRef && a_data.actionRef->data.objectReference)
		{
			auto perk = reinterpret_cast<RE::BGSPerk*>(a_param1);
			if (!perk)
			{
				return true;
			}

			switch (a_data.actionRef->data.objectReference->GetFormType())
			{
			case RE::ENUM_FORM_ID::kSPEL:
				if (auto spell = a_data.actionRef->data.objectReference->As<RE::SpellItem>())
				{
					if (spell->data.castingPerk == perk)
					{
						a_returnValue = 1.0f;
						return true;
					}
				}
				break;

			case RE::ENUM_FORM_ID::kNPC_:
				if (auto conds = perk->perkConditions)
				{
					auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
					if (conds.IsTrue(PlayerCharacter, PlayerCharacter))
					{
						a_returnValue = 1.0f;
						return true;
					}
				}
				break;
			}
		}

		return true;
	}

	inline void Install()
	{
		if (auto func = RE::SCRIPT_FUNCTION::LocateScriptCommand("SpellHasCastingPerk"sv))
		{
			func->executeFunction = &Execute;
			func->conditionFunction = &Condition;
		}
	}
}
