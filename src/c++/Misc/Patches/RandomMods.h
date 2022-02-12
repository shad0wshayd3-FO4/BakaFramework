#pragma once

#include "effolkronium/random.hpp"

namespace Patches::RandomMods
{
	namespace
	{
		using Random = effolkronium::random_thread_local;

		std::atomic<bool> bRandom{ false };

		static inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sScrapItem{ REL::ID(528692) };
		static inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sCannotScrap{ REL::ID(638835) };

		class hkApplyChangesFunctor_w :
			public RE::ApplyChangesFunctor
		{
		public:
			hkApplyChangesFunctor_w(RE::BGSInventoryItem::StackDataWriteFunctor& a_rhs)
			{
				if (auto data = static_cast<RE::ApplyChangesFunctor*>(&a_rhs))
				{
					shouldSplitStacks = data->shouldSplitStacks;
					transferEquippedToSplitStack = data->transferEquippedToSplitStack;
					extra = data->extra;
					object = data->object;
					mod = data->mod;
					rank = data->rank;
					remove = data->remove;
					excludeTemporary = data->excludeTemporary;
					favoriteIndex = data->favoriteIndex;
				}
			}

			virtual void WriteDataImpl(RE::TESBoundObject& a_baseObj, RE::BGSInventoryItem::Stack& a_stack) override
			{
				mod = GetRandomMod(mod, a_stack);
				__super::WriteDataImpl(a_baseObj, a_stack);
			}

		private:
			using List_t = std::vector<const RE::BGSMod::Attachment::Mod*>;

			void GetModListImpl(const RE::BGSMod::Attachment::Mod* a_mod, List_t& a_modList)
			{
				if (!a_mod)
				{
					return;
				}

				if (a_mod->formFlags & 0x80)
				{
					RE::BGSMod::Attachment::Mod::Data data;
					a_mod->GetData(data);

					if (data.attachmentCount > 0)
					{
						for (std::uint32_t i = 0; i < data.attachmentCount; i++)
						{
							if (data.attachments[i].mod)
							{
								GetModListImpl(data.attachments[i].mod, a_modList);
							}
						}
					}

					return;
				}

				a_modList.emplace_back(a_mod);
			}

			auto GetModList(const RE::BGSMod::Attachment::Mod* a_mod)
			{
				List_t result;
				GetModListImpl(a_mod, result);
				return result;
			}

			const RE::BGSMod::Attachment::Mod*
				GetRandomMod(const RE::BGSMod::Attachment::Mod* a_mod, RE::BGSInventoryItem::Stack& a_stack)
			{
				auto modList = GetModList(a_mod);
				if (modList.size() > 1)
				{
					bRandom = true;
				}

				if (a_stack.extra)
				{
					auto idx = std::find(modList.begin(), modList.end(), a_stack.extra->GetLegendaryMod());
					if (idx != modList.end())
					{
						modList.erase(idx);
					}
				}

				std::size_t minRoll = 0;
				std::size_t maxRoll = modList.size() - 1;
				auto randomIdx = Random::get(minRoll, maxRoll);
				return modList[randomIdx];
			}
		};

		class hkApplyChangesFunctor_r :
			public RE::ApplyChangesFunctor
		{
		public:
			hkApplyChangesFunctor_r(RE::BGSInventoryItem::StackDataWriteFunctor& a_rhs)
			{
				if (auto data = static_cast<RE::ApplyChangesFunctor*>(&a_rhs))
				{
					shouldSplitStacks = data->shouldSplitStacks;
					transferEquippedToSplitStack = data->transferEquippedToSplitStack;
					extra = data->extra;
					object = data->object;
					mod = data->mod;
					rank = data->rank;
					remove = data->remove;
					excludeTemporary = data->excludeTemporary;
					favoriteIndex = data->favoriteIndex;
				}
			}

			virtual void WriteDataImpl(RE::TESBoundObject& a_baseObj, RE::BGSInventoryItem::Stack& a_stack) override
			{
				if (mod && mod->formFlags & 0x80)
				{
					if (a_stack.extra)
					{
						mod = a_stack.extra->GetLegendaryMod();
					}
				}

				__super::WriteDataImpl(a_baseObj, a_stack);
			}
		};

		inline void hkFindAndWriteStackDataForInventoryItem(
			RE::TESObjectREFR* a_this,
			RE::TESBoundObject* a_object,
			RE::BGSInventoryItem::StackDataCompareFunctor& a_compareFunc,
			RE::BGSInventoryItem::StackDataWriteFunctor& a_writeFunc,
			bool (*a_objCompFn)(RE::TESBoundObject*, RE::TESBoundObject*),
			bool a_alwaysContinue)
		{
			auto writeFunc = hkApplyChangesFunctor_w(a_writeFunc);
			if (a_this && a_this->inventoryList)
			{
				a_this->inventoryList->FindAndWriteStackDataForItem(a_object, a_compareFunc, writeFunc, a_objCompFn, a_alwaysContinue);
			}
		}

		inline bool hkFindAndWriteStackData(
			RE::BGSInventoryItem* a_this,
			RE::BGSInventoryItem::StackDataCompareFunctor& a_compareFunc,
			RE::BGSInventoryItem::StackDataWriteFunctor& a_writeFunc,
			bool a_manualMerge,
			RE::ObjectRefHandle a_owner)
		{
			auto writeFunc = hkApplyChangesFunctor_r(a_writeFunc);
			if (a_this)
			{
				return a_this->FindAndWriteStackData(a_compareFunc, writeFunc, a_manualMerge, a_owner);
			}

			return false;
		}

		inline REL::Relocation<decltype(&RE::ExamineMenu::BuildConfirmed)> ogBuildConfirmed;
		inline void hkBuildConfirmed(RE::ExamineMenu* a_this, bool a_ownerIsWorkbench)
		{
			ogBuildConfirmed(a_this, a_ownerIsWorkbench);

			if (bRandom)
			{
				RE::Scaleform::GFx::Value args[2];
				args[0] = "Cancel";
				args[1] = false;
				a_this->menuObj.Invoke("ProcessUserEvent", nullptr, args, 2);
				bRandom = false;
			}
		}

		inline REL::Relocation<decltype(&RE::ExamineMenu::Call)> ogCall;
		inline void hkCall(RE::ExamineMenu* a_this, const RE::Scaleform::GFx::FunctionHandler::Params& a_params)
		{
			switch (reinterpret_cast<std::uint64_t>(a_params.userData))
			{
				case 30:
					if (a_this->moddedInventoryItem.object)
					{
						auto hasKeyword{ false }, hasLgndMod{ false };

						if (auto DFOB = RE::TESForm::GetFormByEditorID<RE::BGSDefaultObject>("UnscrappableObject_DO"))
						{
							if (auto KYWD = DFOB->GetForm<RE::BGSKeyword>())
							{
								switch (a_this->moddedInventoryItem.object->GetFormType())
								{
									case RE::ENUM_FORM_ID::kARMO:
										if (auto ARMO = a_this->moddedInventoryItem.object->As<RE::TESObjectARMO>())
										{
											hasKeyword = ARMO->HasKeyword(KYWD, a_this->moddedInventoryItem.GetInstanceData(0));
											if (a_this->moddedInventoryItem.stackData && a_this->moddedInventoryItem.stackData->extra)
											{
												hasLgndMod = a_this->moddedInventoryItem.stackData->extra->GetLegendaryMod();
											}
										}
										break;

									case RE::ENUM_FORM_ID::kWEAP:
										if (auto WEAP = a_this->moddedInventoryItem.object->As<RE::TESObjectWEAP>())
										{
											hasKeyword = WEAP->HasKeyword(KYWD, a_this->moddedInventoryItem.GetInstanceData(0));
											if (a_this->moddedInventoryItem.stackData && a_this->moddedInventoryItem.stackData->extra)
											{
												hasLgndMod = a_this->moddedInventoryItem.stackData->extra->GetLegendaryMod();
											}
										}
										break;

									default:
										stl::report_and_fail("Unhandled FormType exception"sv);
								}
							}
						}

						if (hasKeyword && !hasLgndMod)
						{
							RE::SendHUDMessage::ShowHUDMessage(
								sCannotScrap->GetString().data(),
								"",
								true,
								true);
						}
						else
						{
							a_this->BuildWeaponScrappingArray();
							if (auto callback = new RE::ScrapItemCallback(a_this, a_this->GetSelectedIndex()))
							{
								if (auto dataScrap =
										new RE::ExamineConfirmMenu::InitDataScrap(
											sScrapItem->GetString().data(),
											"$SCRAP",
											a_this->moddedInventoryItem.GetDisplayFullName(0),
											a_this->scrappingArray))
								{
									a_this->ShowConfirmMenu(dataScrap, callback);
								}
							}
						}
					}
					break;

				default:
					return ogCall(a_this, a_params);
			}
		}
	}

	void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> targetWSDW{ REL::ID(1396668), 0x4DF };
		REL::Relocation<std::uintptr_t> targetWSDR{ REL::ID(289994), 0x24C };
		auto& trampoline = F4SE::GetTrampoline();
		trampoline.write_call<5>(targetWSDW.address(), hkFindAndWriteStackDataForInventoryItem);
		trampoline.write_call<5>(targetWSDR.address(), hkFindAndWriteStackData);

		REL::Relocation<std::uintptr_t> targetVtbl{ RE::ExamineMenu::VTABLE[0] };
		ogCall = targetVtbl.write_vfunc(0x01, hkCall);
		ogBuildConfirmed = targetVtbl.write_vfunc(0x17, hkBuildConfirmed);
	}
}
