#include "SteamAPI.h"

#include <shellapi.h>
#include <steam/steam_api.h>

namespace SteamAPI
{
    namespace detail
    {
        ISteamFriends* SteamFriends()
        {
            auto handle = GetModuleHandleW(L"steam_api64");
            if (handle == NULL)
            {
                return nullptr;
            }

            using func_t = decltype(&::SteamFriends);
            auto func =
                reinterpret_cast<func_t>(GetProcAddress(handle, "SteamFriends"));
            return (func != NULL) ? func() : nullptr;
        }

        ISteamUtils* SteamUtils()
        {
            auto handle = GetModuleHandleW(L"steam_api64");
            if (handle == NULL)
            {
                return nullptr;
            }

            using func_t = decltype(&::SteamUtils);
            auto func =
                reinterpret_cast<func_t>(GetProcAddress(handle, "SteamUtils"));
            return (func != NULL) ? func() : nullptr;
        }
    }

    void OpenWebPage(const char* a_url, bool a_fallback)
    {
        auto utils = detail::SteamUtils();
        if (utils && utils->IsOverlayEnabled())
        {
            auto friends = detail::SteamFriends();
            if (friends)
            {
                friends->ActivateGameOverlayToWebPage(a_url);
            }
        }

        if (a_fallback)
        {
            DEBUG("Overlay is disabled."sv);
            ShellExecuteA(NULL, NULL, a_url, NULL, NULL, SW_MAXIMIZE);
        }
    }
}
