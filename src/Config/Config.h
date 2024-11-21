#pragma once

namespace Config
{
	namespace Patches
	{
		static REX::INI::Bool bEnableLoadingEditorIDs{ "bEnableLoadingEditorIDs", "Patches", true };
		static REX::INI::Bool bEnableEDIDConflictCheck{ "bEnableEDIDConflictCheck", "Patches", false };
		static REX::INI::Str sBetaCommentFileName{ "sBetaCommentFileName", "Patches", "BetaComment.txt"s };
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
