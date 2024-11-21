#pragma once

namespace Forms
{
	static RE::BGSDefaultObject* InventoryWeight_DO{ nullptr };
	static RE::BGSDefaultObject* PipboyLightActive_DO{ nullptr };
	static RE::BGSDefaultObject* RadiationSourceCount_DO{ nullptr };

	void Install()
	{
		InventoryWeight_DO =
			RE::DefaultObjectFormFactory::Create(
				"InventoryWeight_DO",
				RE::ENUM_FORM_ID::kAVIF);

		PipboyLightActive_DO =
			RE::DefaultObjectFormFactory::Create(
				"PipboyLightActive_DO",
				RE::ENUM_FORM_ID::kAVIF);

		RadiationSourceCount_DO =
			RE::DefaultObjectFormFactory::Create(
				"RadiationSourceCount_DO",
				RE::ENUM_FORM_ID::kAVIF);

		F4SE::log::debug("Injected DefaultObjects."sv);
	}
};
