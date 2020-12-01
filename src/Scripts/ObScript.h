#pragma once

#include "Scripts/ObScript/BetaComment.h"
#include "Scripts/ObScript/GetBaseObject.h"
#include "Scripts/ObScript/QuitGame.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::GetBaseObject::Install();
		ObScript::QuitGame::Install();
	}
}