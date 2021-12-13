#pragma once

#include "SteamAPI/SteamAPI.h"

namespace Papyrus
{
	namespace BakaUtil
	{
		constexpr auto SCRIPT_NAME = "BakaUtil"sv;

		std::vector<RE::TESObjectREFR*> FilterReferencesByKeywords(std::monostate, std::vector<RE::TESObjectREFR*> a_refrs, std::vector<RE::BGSKeyword*> a_kywds, bool a_whitelist)
		{
			std::vector<RE::TESObjectREFR*> result;
			if (a_refrs.size() == 0 && a_kywds.size() == 0)
			{
				return result;
			}

			for (auto refr : a_refrs)
			{
				if (!refr)
				{
					continue;
				}

				for (auto kywd : a_kywds)
				{
					if (!kywd)
					{
						continue;
					}

					auto hasKeyword = refr->HasKeyword(kywd);
					if (a_whitelist && hasKeyword)
					{
						result.emplace_back();
					}
				}
			}

			return result;
		}

		void OpenWebPage(std::monostate, std::string_view a_url, bool a_fallback)
		{
			SteamAPI::OpenWebPage(a_url.data(), a_fallback);
		}
	}
}
