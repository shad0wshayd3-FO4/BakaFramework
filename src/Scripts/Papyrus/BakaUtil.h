#pragma once

namespace Papyrus::BakaUtil
{
	std::vector<RE::TESObjectREFR*> FilterReferencesByKeywords(
		std::monostate,
		std::vector<RE::TESObjectREFR*> a_refrs,
		std::vector<RE::BGSKeyword*> a_kywds,
		bool a_whitelist)
	{
		std::vector<RE::TESObjectREFR*> result;
		if (a_refrs.size() == 0 || a_kywds.size() == 0)
		{
			return result;
		}

		for (auto& refr : a_refrs)
		{
			if (refr)
			{
				for (auto& kywd : a_kywds)
				{
					if (kywd)
					{
						if (a_whitelist && refr->HasKeyword(kywd))
						{
							result.emplace_back();
						}
					}
				}
			}
		}

		return result;
	}
}
