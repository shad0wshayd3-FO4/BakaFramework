#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/FreePlayer.h"
#include "ObScript/GetBaseObject.h"
#include "ObScript/GetDefaultObject.h"
#include "ObScript/QuitGame.h"
#include "ObScript/SpellHasCastingPerk.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::FreePlayer::Install();
		ObScript::GetBaseObject::Install();
		ObScript::GetDefaultObject::Install();
		ObScript::QuitGame::Install();
		ObScript::SpellHasCastingPerk::Install();
	}
}
