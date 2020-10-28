#pragma once

class Forms
{
private:
	static std::uint32_t HookInitializer_DefaultObject()
	{
		// Initializer override
		DefaultPreviewTransform_DO =
			RE::DefaultObjectFormFactory::Create(
				"DefaultPreviewTransform_DO",
				"This transform is the one used for preview on all forms which do not have a hand tagged transform.",
				RE::ENUM_FORM_ID::kTRNS);

		// Add new
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

		logger::debug("Injected DefaultObjects."sv);
		return 1;
	}

	static std::uint32_t HookInitializer_Setting()
	{
		RE::GameSettingCollection::InitCollection();
		if (auto GameSettingCollection = RE::GameSettingCollection::GetSingleton(); GameSettingCollection)
		{
			// Initializer override
			GameSettingCollection->Add(&fBlockMax);

			// Add new
			// TODO
		}

		logger::debug("Injected GMSTs."sv);
		return 1;
	}

	// members
	inline static RE::Setting fBlockMax{ "fBlockMax", 0.7f };
	inline static RE::BGSDefaultObject* DefaultPreviewTransform_DO{ nullptr };

public:
	static void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> target_DefaultObject{ REL::ID(1389727) };
		REL::Relocation<std::uintptr_t> target_Setting{ REL::ID(1078413) };

		auto& trampoline = F4SE::GetTrampoline();
		trampoline.write_branch<6>(target_DefaultObject.address(), HookInitializer_DefaultObject);
		trampoline.write_branch<5>(target_Setting.address(), HookInitializer_Setting);
	}

	// members
	inline static RE::BGSDefaultObject* InventoryWeight_DO{ nullptr };
	inline static RE::BGSDefaultObject* PipboyLightActive_DO{ nullptr };
	inline static RE::BGSDefaultObject* RadiationSourceCount_DO{ nullptr };
};