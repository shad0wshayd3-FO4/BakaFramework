#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/GetBaseObject.h"
#include "ObScript/GetDefaultObject.h"
#include "ObScript/Help.h"
#include "ObScript/SpellHasCastingPerk.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::GetBaseObject::Install();
		ObScript::GetDefaultObject::Install();
		ObScript::Help::Install();
		ObScript::SpellHasCastingPerk::Install();
	}
}
