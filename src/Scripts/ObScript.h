#pragma once

#include "Scripts/ObScript/BetaComment.h"
#include "Scripts/ObScript/QuitGame.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::QuitGame::Install();
	}
}