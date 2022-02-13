#pragma once

namespace Forms
{
	RE::BGSDefaultObject* InventoryWeight_DO{ nullptr };
	RE::BGSDefaultObject* PipboyLightActive_DO{ nullptr };
	RE::BGSDefaultObject* RadiationSourceCount_DO{ nullptr };
	RE::BGSDefaultObject* PAFrameWorkshop_DO{ nullptr };
	RE::BGSDefaultObject* PAFrameToken_DO{ nullptr };
	RE::BGSDefaultObject* PAFramePerk_DO{ nullptr };
	RE::BGSDefaultObject* PAFrameMessage_DO{ nullptr };

	namespace
	{
		RE::Setting fBlockMax{ "fBlockMax", 0.7f };
		RE::BGSDefaultObject* DefaultPreviewTransform_DO{ nullptr };

		std::uint32_t hkDefaultObject()
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

			PAFrameWorkshop_DO =
				RE::DefaultObjectFormFactory::Create(
					"PAFrameWorkshop_DO",
					RE::ENUM_FORM_ID::kCONT);

			PAFrameToken_DO =
				RE::DefaultObjectFormFactory::Create(
					"PAFrameToken_DO",
					RE::ENUM_FORM_ID::kARMO);

			PAFramePerk_DO =
				RE::DefaultObjectFormFactory::Create(
					"PAFramePerk_DO",
					RE::ENUM_FORM_ID::kPERK);

			PAFrameMessage_DO =
				RE::DefaultObjectFormFactory::Create(
					"PAFrameMessage_DO",
					RE::ENUM_FORM_ID::kMESG);

			logger::debug("Injected DefaultObjects."sv);
			return 1;
		}

		std::uint32_t hkGameSetting()
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
	}

	void Install()
	{
		REL::Relocation<std::uintptr_t> targetDFOB{ REL::ID(1389727) };
		REL::Relocation<std::uintptr_t> targetGMST{ REL::ID(1078413) };
		stl::asm_replace(targetDFOB.address(), 0x31, reinterpret_cast<std::uintptr_t>(hkDefaultObject));
		stl::asm_replace(targetGMST.address(), 0x68, reinterpret_cast<std::uintptr_t>(hkGameSetting));
	}
};
