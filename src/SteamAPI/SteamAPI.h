#pragma once

#include <steam/steam_api.h>

class SteamAPI
{
public:
	static void Init();

	static bool IsOverlayEnabled()
	{
		return _utils ? _utils->IsOverlayEnabled() : false;
	}

	static void OpenWebPage(const char* a_URL, bool a_fallbackToBrowser = false)
	{
		if (IsOverlayEnabled())
		{
			if (_friends)
			{
				_friends->ActivateGameOverlayToWebPage(a_URL);
				return;
			}
		}

		logger::debug("Steam overlay is disabled, or _utils is null");
		if (a_fallbackToBrowser)
		{
			OpenURLInBrowser(a_URL);
		}
	}

private:
	static void OpenURLInBrowser(const char* a_URL);

protected:
	static inline ISteamFriends* _friends{ nullptr };
	static inline ISteamUtils* _utils{ nullptr };
};