#pragma once

namespace Forms
{
	// static RE::ActorValueInfo* InventoryWeight{ nullptr };
	static RE::ActorValueInfo* PipboyLightActive{ nullptr };
	static RE::ActorValueInfo* RadiationSourceCount{ nullptr };

	void Install()
	{
		if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
		{
			PipboyLightActive = TESDataHandler->LookupForm<RE::ActorValueInfo>(0x800, "BakaFramework.esm"sv);
			RadiationSourceCount = TESDataHandler->LookupForm<RE::ActorValueInfo>(0x801, "BakaFramework.esm"sv);
		}
	}
};
