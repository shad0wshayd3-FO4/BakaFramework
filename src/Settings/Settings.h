#pragma once

namespace Settings
{
	namespace Patches
	{
		static REX::TIniSetting<bool>        bEnableLoadingEditorIDs{ "Patches"sv, "bEnableLoadingEditorIDs"sv, true };
		static REX::TIniSetting<bool>        bEnableEDIDConflictCheck{ "Patches"sv, "bEnableEDIDConflictCheck"sv, false };
		static REX::TIniSetting<std::string> sBetaCommentFileName{ "Patches"sv, "sBetaCommentFileName"sv, "BetaComment.txt"s };
	}

	static void Load()
	{
		const auto ini = REX::FIniSettingStore::GetSingleton();
		ini->Init(
			"Data/F4SE/plugins/BakaFramework.ini",
			"Data/F4SE/plugins/BakaFrameworkCustom.ini");
		ini->Load();
	}
}
