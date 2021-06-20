#include "SteamAPI.h"
#include <windows.h>

void SteamAPI::OpenURLInBrowser(const char* a_URL)
{
	ShellExecuteA(NULL, NULL, a_URL, NULL, NULL, SW_MAXIMIZE);
}

void SteamAPI::Init()
{
	auto handle = GetModuleHandle(L"steam_api64");
	if (handle == NULL)
	{
		logger::error("steam_api64 handle is null");
		return;
	}

	// SteamFriends
	{
		using func_t = ISteamFriends* (*)();
		auto func = (func_t)GetProcAddress(handle, "SteamFriends");
		if (func != NULL)
		{
			_friends = func();
			logger::debug("Stored SteamAPI::_friends");
		}
	}

	// SteamUtils
	{
		using func_t = ISteamUtils* (*)();
		auto func = (func_t)GetProcAddress(handle, "SteamUtils");
		if (func != NULL)
		{
			_utils = func();
			logger::debug("Stored SteamAPI::_utils");
		}
	}
}
