#pragma once

namespace Config
{
	namespace Patches
	{
		static REX::INI::Bool bEnableLoadingEditorIDs{ "Patches", "bEnableLoadingEditorIDs", true };
		static REX::INI::Bool bEnableEDIDConflictCheck{ "Patches", "bEnableEDIDConflictCheck", false };
		static REX::INI::Str  sBetaCommentFileName{ "Patches", "sBetaCommentFileName", "BetaComment.txt"s };
	}

	static void Load()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"Data/F4SE/plugins/BakaFramework.ini",
			"Data/F4SE/plugins/BakaFrameworkCustom.ini");
		ini->Load();
	}
}
