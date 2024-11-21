#pragma once

namespace ObScript
{
	class SpellHasCastingPerk
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
					return _stricmp(a_elem.functionName, LONG_NAME.data()) == 0;
				});

			if (it != functions.end())
			{
				static std::array params{
					RE::SCRIPT_PARAMETER{"Perk", RE::SCRIPT_PARAM_TYPE::kPerk, false},
				};

				*it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
				it->paramCount = static_cast<std::uint16_t>(params.size());
				it->parameters = params.data();
				it->executeFunction = Execute;
				it->conditionFunction = Condition;

				DEBUG("Registered SpellHasCastingPerk."sv);
			}
			else
			{
				DEBUG("Failed to register SpellHasCastingPerk."sv);
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
			return false;
		}

		static bool Condition(
			RE::ConditionCheckParams& a_params,
			void* a_perk,
			void*,
			float& a_returnValue)
		{
			a_returnValue = 0.0f;
			if (a_params.actionRef && a_params.actionRef->data.objectReference)
			{
				auto PERK = reinterpret_cast<RE::BGSPerk*>(a_perk);
				if (!PERK)
				{
					return true;
				}

				switch (a_params.actionRef->data.objectReference->GetFormType())
				{
				case RE::ENUM_FORM_ID::kSPEL:
					if (auto SPEL = a_params.actionRef->data.objectReference->As<RE::SpellItem>())
					{
						if (SPEL->data.castingPerk == PERK)
						{
							a_returnValue = 1.0f;
							return true;
						}
					}
					break;

				case RE::ENUM_FORM_ID::kNPC_:
					if (auto COND = PERK->perkConditions)
					{
						auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
						if (COND.IsTrue(PlayerCharacter, PlayerCharacter))
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

		static constexpr auto LONG_NAME = "SpellHasCastingPerk"sv;
		static constexpr auto SHORT_NAME = ""sv;
	};
}
