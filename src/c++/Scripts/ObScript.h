#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/FreePlayer.h"
#include "ObScript/GetBaseObject.h"
#include "ObScript/GetDefaultObject.h"
#include "ObScript/SpellHasCastingPerk.h"
#include "ObScript/QuitGame.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::FreePlayer::Install();
		ObScript::GetBaseObject::Install();
		ObScript::GetDefaultObject::Install();
		ObScript::SpellHasCastingPerk::Install();
		ObScript::QuitGame::Install();
	}
}
