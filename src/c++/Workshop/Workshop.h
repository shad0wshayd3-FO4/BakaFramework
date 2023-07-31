/*
#pragma once

namespace Workshop
{
	class PlacementMode :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::Workshop::ItemPlacedEvent>,
		public RE::BSTEventSink<RE::Workshop::WorkshopModeEvent>
	{
	public:
		class Hooks
		{
		public:
			template<std::uint64_t id, std::ptrdiff_t offset>
			class hkCanNavigate
			{
			public:
				static void Install()
				{
					REL::Relocation<std::uintptr_t> target{ REL::ID(id), offset };

					auto& trampoline = F4SE::GetTrampoline();
					func = trampoline.write_call<5>(target.address(), thunk);
				}

			private:
				static bool thunk()
				{
					return PlacementMode::IsActive() ? false : func();
				}

				inline static REL::Relocation<decltype(&thunk)> func;
			};

			template<std::uint64_t id, std::ptrdiff_t offset>
			class hkIsReferenceWithinBuildableArea
			{
			public:
				static void Install()
				{
					REL::Relocation<std::uintptr_t> target{ REL::ID(id), offset };

					auto& trampoline = F4SE::GetTrampoline();
					func = trampoline.write_call<5>(target.address(), thunk);
				}

			private:
				static bool thunk(const RE::TESObjectREFR& a_workshop, const RE::TESObjectREFR& a_refr)
				{
					if (PlacementMode::IsActive())
					{
						return true;
					}

					if (a_workshop.formFlags & 0x20)
					{
						return false;
					}

					return func(a_workshop, a_refr);
				}

				inline static REL::Relocation<decltype(&thunk)> func;
			};

			template<std::uint64_t id, std::ptrdiff_t offset>
			class hkPlayMenuSound
			{
			public:
				static void Install()
				{
					REL::Relocation<std::uintptr_t> target{ REL::ID(id), offset };

					auto& trampoline = F4SE::GetTrampoline();
					func = trampoline.write_call<5>(target.address(), thunk);
				}

			private:
				static bool thunk(const char* a_soundName)
				{
					if (PlacementMode::IsActive())
					{
						PlacementMode::GetSingleton()->m_hasSound = true;
						return func("");
					}

					if (PlacementMode::GetSingleton()->m_hasSound)
					{
						if (_stricmp(a_soundName, "UIWorkshopModeExit") == 0)
						{
							PlacementMode::GetSingleton()->m_hasSound = false;
							return func("");
						}
					}

					return func(a_soundName);
				}

				inline static REL::Relocation<decltype(&thunk)> func;
			};

			template<std::uint64_t id, std::ptrdiff_t offset>
			class hkShouldShowTagForSearch
			{
			public:
				static void Install()
				{
					REL::Relocation<std::uintptr_t> target{ REL::ID(id), offset };

					auto& trampoline = F4SE::GetTrampoline();
					func = trampoline.write_call<5>(target.address(), thunk);
				}

			private:
				static std::uint64_t thunk(RE::WorkshopMenu* a_this)
				{
					return PlacementMode::IsActive() ? 0 : func(a_this);
				}

				inline static REL::Relocation<decltype(&thunk)> func;
			};

			template<std::uint64_t id, std::ptrdiff_t offset>
			class hkUpdateRequirements
			{
			public:
				static void Install()
				{
					REL::Relocation<std::uintptr_t> target{ REL::ID(id), offset };

					auto& trampoline = F4SE::GetTrampoline();
					func = trampoline.write_call<5>(target.address(), thunk);
				}

			private:
				static void thunk(RE::WorkshopMenu* a_this, bool a_stringingWire)
				{
					if (PlacementMode::IsActive())
					{
						return;
					}

					func(a_this, a_stringingWire);
				}

				inline static REL::Relocation<decltype(&thunk)> func;
			};

			static void Install()
			{
				// Prevent stored frames from stacking
				REL::Relocation<std::uintptr_t> targetCMP{ RE::ExtraStartingWorldOrCell::VTABLE[0] };
				targetCMP.write_vfunc(0x01, hkCompareImpl);

				// Redirect Cancel input, block other buttons
				REL::Relocation<std::uintptr_t> targetEVN{ RE::WorkshopMenu::VTABLE[1] };
				ogHandleEvent = targetEVN.write_vfunc(0x08, hkHandleEvent);

				// Disable selection in UI
				hkCanNavigate<119865, 0x37A>::Install();
				hkCanNavigate<119865, 0x3EA>::Install();
				hkCanNavigate<119865, 0x4C5>::Install();
				hkCanNavigate<119865, 0x768>::Install();
				hkCanNavigate<985073, 0x0C>::Install();
				hkCanNavigate<1130413, 0x0C>::Install();

				// Prevent Workshops marked as deleted as being valid
				hkIsReferenceWithinBuildableArea<2562, 0x156>::Install();
				hkIsReferenceWithinBuildableArea<978467, 0x07E>::Install();
				hkIsReferenceWithinBuildableArea<90862, 0x039>::Install();
				hkIsReferenceWithinBuildableArea<939377, 0x090>::Install();
				hkIsReferenceWithinBuildableArea<552874, 0x024>::Install();
				hkIsReferenceWithinBuildableArea<525394, 0x017>::Install();
				hkIsReferenceWithinBuildableArea<311311, 0x0A6>::Install();
				hkIsReferenceWithinBuildableArea<566990, 0x05C>::Install();
				hkIsReferenceWithinBuildableArea<286947, 0x047>::Install();
				hkIsReferenceWithinBuildableArea<1371490, 0x091>::Install();
				hkIsReferenceWithinBuildableArea<931840, 0x165>::Install();
				hkIsReferenceWithinBuildableArea<1515428, 0x036>::Install();

				// Disable Workshop Startup/End sounds
				hkPlayMenuSound<598489, 0x1195>::Install();
				hkPlayMenuSound<98443, 0x01B5>::Install();

				// Prevent tagging for search in FreeBuild mode
				hkShouldShowTagForSearch<119865, 0xEBB>::Install();
				hkShouldShowTagForSearch<1089189, 0x574>::Install();

				// Prevent a stupid textbox from showing up for a split second after placing an item
				hkUpdateRequirements<598489, 0x1144>::Install();
				hkUpdateRequirements<1280212, 0x1F0>::Install();
				hkUpdateRequirements<119865, 0x05C5>::Install();
				hkUpdateRequirements<119865, 0x0868>::Install();
				hkUpdateRequirements<119865, 0x0A16>::Install();
				hkUpdateRequirements<119865, 0x0A42>::Install();
				hkUpdateRequirements<119865, 0x0A65>::Install();
				hkUpdateRequirements<119865, 0x0A88>::Install();
				hkUpdateRequirements<119865, 0x0C64>::Install();
				hkUpdateRequirements<119865, 0x0CD4>::Install();
				hkUpdateRequirements<119865, 0x0F53>::Install();
				hkUpdateRequirements<119865, 0x10FD>::Install();
				hkUpdateRequirements<119865, 0x1181>::Install();
				hkUpdateRequirements<119865, 0x13F6>::Install();
				hkUpdateRequirements<119865, 0x1764>::Install();

				// Enable ExtraStartingWorldOrCell as a stacking condition
				REL::Relocation<std::uintptr_t> targetUIQ{ REL::ID(179412) };
				stl::asm_replace(targetUIQ.address(), 0x1C7, reinterpret_cast<std::uintptr_t>(hkUIQualifier));
			}

			inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> sPADisallowed{ REL::ID(1053596) };

		private:
			static bool hkCompareImpl(RE::ExtraStartingWorldOrCell* a_this, const RE::ExtraStartingWorldOrCell& a_compare)
			{
				if (!a_this)
				{
					return true;
				}

				if (a_this->type != RE::ExtraStartingWorldOrCell::TYPE || a_compare.type != RE::ExtraStartingWorldOrCell::TYPE)
				{
					return true;
				}

				if (a_this->startingWorldOrCell->GetFormType() == RE::ENUM_FORM_ID::kCELL || a_this->startingWorldOrCell->GetFormType() == RE::ENUM_FORM_ID::kWRLD || a_compare.startingWorldOrCell->GetFormType() == RE::ENUM_FORM_ID::kCELL || a_compare.startingWorldOrCell->GetFormType() == RE::ENUM_FORM_ID::kWRLD)
				{
					return false;
				}

				return a_this->startingWorldOrCell != a_compare.startingWorldOrCell;
			}

			static void hkHandleEvent(RE::BSInputEventUser* a_this, const RE::ButtonEvent* a_event)
			{
				if (a_event && PlacementMode::IsActive())
				{
					if (a_event->QUserEvent() == "XButton" || a_event->QUserEvent() == "YButton" || a_event->QUserEvent() == "LShoulder" || a_event->QUserEvent() == "RShoulder" || a_event->QUserEvent() == "LTrigger" || a_event->QUserEvent() == "RTrigger" || a_event->QUserEvent() == "Sprint" || a_event->QUserEvent() == "Jump")
					{
						return;
					}

					if (a_event->QUserEvent() == "Cancel")
					{
						auto _event =
							stl::unrestricted_cast<RE::ButtonEvent*>(a_event);
						_event->strUserEvent = "CloseMenu";
						return ogHandleEvent(a_this, _event);
					}
				}

				ogHandleEvent(a_this, a_event);
			}

			static bool hkUIQualifier(const RE::BSExtraData* a_extra)
			{
				if (!a_extra)
				{
					return false;
				}

				switch (a_extra->type.get())
				{
					// case RE::EXTRA_DATA_TYPE::kPersistentCell:
					// case RE::EXTRA_DATA_TYPE::kKeywords:
					// case RE::EXTRA_DATA_TYPE::kStartingPosition:
					// case RE::EXTRA_DATA_TYPE::kReferenceHandle:
					// case RE::EXTRA_DATA_TYPE::kOwnership:
					// case RE::EXTRA_DATA_TYPE::kGlobal:
					// case RE::EXTRA_DATA_TYPE::kRank:
					case RE::EXTRA_DATA_TYPE::kHealth:
					// case RE::EXTRA_DATA_TYPE::kTimeLeft:
					case RE::EXTRA_DATA_TYPE::kCharge:
					// case RE::EXTRA_DATA_TYPE::kLevelItem:
					// case RE::EXTRA_DATA_TYPE::kScale:
					case RE::EXTRA_DATA_TYPE::kObjectInstance:
					case RE::EXTRA_DATA_TYPE::kCannotWear:
					case RE::EXTRA_DATA_TYPE::kPoison:
					case RE::EXTRA_DATA_TYPE::kBoundArmor:
					case RE::EXTRA_DATA_TYPE::kStartingWorldOrCell:
					// case RE::EXTRA_DATA_TYPE::kFavorite:
					// case RE::EXTRA_DATA_TYPE::kAliasInstanceArray:
					// case RE::EXTRA_DATA_TYPE::kPromotedRef:
					// case RE::EXTRA_DATA_TYPE::kOutfitItem:
					// case RE::EXTRA_DATA_TYPE::kFromAlias:
					// case RE::EXTRA_DATA_TYPE::kShouldWear:
					case RE::EXTRA_DATA_TYPE::kTextDisplayData:
					case RE::EXTRA_DATA_TYPE::kEnchantment:
					// case RE::EXTRA_DATA_TYPE::kUniqueID:
					// case RE::EXTRA_DATA_TYPE::kFlags:
					case RE::EXTRA_DATA_TYPE::kInstanceData:
					case RE::EXTRA_DATA_TYPE::kModRank:
						return true;

					default:
						return false;
				}
			}

			inline static REL::Relocation<decltype(&hkHandleEvent)> ogHandleEvent;
		};

		[[nodiscard]] static PlacementMode* GetSingleton()
		{
			static PlacementMode singleton;
			return std::addressof(singleton);
		}

		static void ApplyPerk()
		{
			auto PAPerk = Forms::PAFramePerk_DO->GetForm<RE::BGSPerk>();
			if (!PAPerk)
			{
				return;
			}

			if (auto Player = RE::TESForm::GetFormByID(0x00000007)->As<RE::TESNPC>())
			{
				Player->AddPerk(PAPerk, 1);
			}
		}

		static void Start()
		{
			if (auto workbench = Forms::PAFrameWorkshop_DO->GetForm<RE::TESObjectCONT>())
			{
				auto singleton = GetSingleton();
				if (singleton->m_workshop = CreateWorkbench(workbench))
				{
					if (auto UI = RE::UI::GetSingleton())
					{
						if (UI->GetMenuOpen("PipboyMenu"))
						{
							RE::UIMessageQueue::GetSingleton()->AddMessage(
								"PipboyMenu",
								RE::UI_MESSAGE_TYPE::kHide);
						}
					}

					singleton->m_isActive = true;
					RE::Workshop::RegisterForWorkshopModeEvent(singleton);
					RE::Workshop::StartWorkshop(singleton->m_workshop.get().get());
				}
			}
		}

		static void SetFrameReference(RE::TESObjectREFR* a_refr)
		{
			GetSingleton()->m_frameRefr.reset(a_refr);
		}

		static void SetTokenReference(RE::TESObjectREFR* a_refr)
		{
			GetSingleton()->m_tokenRefr.reset(a_refr);
		}

		static bool IsActive()
		{
			return GetSingleton()->m_isActive;
		}

		static bool CreateToken(RE::TESObjectREFR* a_refr)
		{
			if (a_refr && a_refr->HasKeyword(RE::TESForm::GetFormByID<RE::BGSKeyword>(0x0003430B)))
			{
				auto token = Forms::PAFrameToken_DO->GetForm<RE::TESObjectARMO>();
				if (!token)
				{
					return false;
				}

				auto extra = RE::BSTSmartPointer(new RE::ExtraDataList());
				extra->SetStartingWorldOrCell(a_refr);

				auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
				if (a_refr->GetHandle() == PlayerCharacter->lastUsedPowerArmor)
				{
					PlayerCharacter->RemoveLastUsedPowerArmor();
				}

				if (auto DPM = RE::BGSDynamicPersistenceManager::GetSingleton())
				{
					DPM->PromoteReference(a_refr, PlayerCharacter);
				}

				const RE::PlayerCharacter::ScopedInventoryChangeMessageContext cmctx{ true, true };
				PlayerCharacter->AddObjectToContainer(
					token,
					extra,
					1,
					nullptr,
					RE::ITEM_REMOVE_REASON::kNone);
				PlayerCharacter->PlayPickUpSound(token, true, false);

				if (auto MESG = Forms::PAFrameMessage_DO->GetForm<RE::BGSMessage>())
				{
					RE::BSFixedString message;
					MESG->GetConvertedDescription(message);
					RE::SendHUDMessage::ShowHUDMessage(message.c_str(), "", true, true);
				}

				a_refr->Disable();
				return true;
			}

			return false;
		}

		static bool HandleToken(RE::TESObjectREFR* a_refr)
		{
			if (RE::PowerArmor::PlayerInPowerArmor())
			{
				const RE::PlayerCharacter::ScopedInventoryChangeMessageContext cmctx{ true, true };
				RE::PlayerCharacter::GetSingleton()->PickUpObject(
					a_refr,
					1,
					false);
				RE::SendHUDMessage::ShowHUDMessage(
					Hooks::sPADisallowed->GetString().data(),
					"",
					true,
					true);
				return false;
			}

			auto token = Forms::PAFrameToken_DO->GetForm<RE::TESObjectARMO>();
			if (!token)
			{
				return false;
			}

			if (a_refr && a_refr->data.objectReference && a_refr->data.objectReference == token)
			{
				if (!a_refr->extraList)
				{
					return false;
				}

				if (auto extra = a_refr->extraList->GetByType<RE::ExtraStartingWorldOrCell>())
				{
					if (auto p_refr = extra->startingWorldOrCell->As<RE::TESObjectREFR>())
					{
						SetTokenReference(a_refr);
						SetFrameReference(p_refr);
						a_refr->Disable();

						Start();
						return true;
					}
				}
			}

			return false;
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_source) override
		{
			if (a_event.menuName == RE::WorkshopMenu::MENU_NAME)
			{
				if (a_event.opening)
				{
					if (auto menu = RE::UI::GetSingleton()->GetMenu<RE::WorkshopMenu>())
					{
						menu->workshopMenuBase->itemName->SetMember("visible", false);
						menu->workshopMenuBase->selectionBracket->SetMember("visible", false);
						menu->workshopMenuBase->itemCounts->SetMember("visible", false);
						menu->workshopMenuBase->newRecipeIcon->SetMember("visible", false);
						menu->workshopMenuBase->rowBrackets->SetMember("visible", false);
						menu->workshopMenuBase->displayPath->SetMember("visible", false);
						menu->workshopMenuBase->descriptionBase->SetMember("visible", false);
						menu->workshopMenuBase->iconBackground->SetMember("visible", false);

						menu->workshopMenuBase->Invoke("HideRequirements");
						menu->workshopMenuBase->Invoke("HideIconCard");
						menu->workshopMenuBase->Invoke("HidePerkPanels");

						(*RE::Workshop::CurrentRow)++;

						menu->CheckAndSetItemForPlacement();
						menu->UpdateButtonText();

						if (auto& handle = *RE::Workshop::PlacementItem)
						{
							if (m_frameRefr && m_frameRefr->inventoryList)
							{
								handle.get()->CreateInventoryList(nullptr);

								const RE::BSAutoReadLock lockerR{ m_frameRefr->inventoryList->rwLock };
								const RE::BSAutoWriteLock lockerW{ handle.get()->inventoryList->rwLock };
								for (auto& iter : m_frameRefr->inventoryList->data)
								{
									handle.get()->inventoryList->data.emplace_back(iter);
								}
							}
						}
					}
				}
				else
				{
					a_source->UnregisterSink(this);
				}
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::Workshop::ItemPlacedEvent& a_event, RE::BSTEventSource<RE::Workshop::ItemPlacedEvent>*) override
		{
			if (a_event.workshop == m_workshop.get())
			{
				if (a_event.placedItem)
				{
					m_frameRefr->SetLocationOnReference(
						a_event.placedItem->data.location);
					m_frameRefr->SetAngleOnReference(
						a_event.placedItem->data.angle);

					if (auto frame3D = m_frameRefr->Get3D())
					{
						m_frameRefr->Update3DPosition(true);
						if (frame3D->flags.flags & 0x4000)
						{
							if (auto node = frame3D->IsFadeNode())
							{
								node->previousMaxA = 1.0f;
								node->currentDecalFade = 1.0f;
								node->currentFade = 1.0f;
								node->flags.flags |= 0x2000000000;
							}
						}
					}

					if (m_frameRefr->parentCell != a_event.placedItem->parentCell)
					{
						m_frameRefr->MoveRefToNewSpace(
							a_event.placedItem->parentCell,
							a_event.placedItem->parentCell->worldSpace);
						m_frameRefr->AddChange(0x400);
					}

					a_event.placedItem->SetDelete(true);
					a_event.placedItem->SetWantsDelete(true);
					a_event.placedItem->Disable();

					m_frameRefr->formFlags |= 0x8000000;
					m_frameRefr->Enable(false);

					if (auto DPM = RE::BGSDynamicPersistenceManager::GetSingleton())
					{
						DPM->DemoteReference(
							m_frameRefr.get(),
							RE::PlayerCharacter::GetSingleton());
					}
				}

				m_tokenRefr->SetDelete(true);
				m_tokenRefr->SetWantsDelete(true);
				m_tokenRefr->Disable();

				RE::Workshop::RequestExitWorkshop(false);
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::Workshop::WorkshopModeEvent& a_event, RE::BSTEventSource<RE::Workshop::WorkshopModeEvent>*) override
		{
			if (a_event.start)
			{
				RE::UI::GetSingleton()->RegisterSink<RE::MenuOpenCloseEvent>(this);
				RE::Workshop::RegisterForItemPlaced(this);
			}
			else
			{
				RE::Workshop::UnregisterForItemPlaced(this);
				RE::Workshop::UnregisterForWorkshopModeEvent(this);

				m_workshop.get()->SetDelete(true);
				m_workshop.get()->SetWantsDelete(true);
				m_workshop.get()->Disable();
				m_workshop.reset();

				if (m_tokenRefr && !m_tokenRefr->GetDelete())
				{
					const RE::PlayerCharacter::ScopedInventoryChangeMessageContext cmctx{ true, true };
					RE::PlayerCharacter::GetSingleton()->AddObjectToContainer(
						m_tokenRefr->data.objectReference,
						m_tokenRefr->extraList,
						1,
						nullptr,
						RE::ITEM_REMOVE_REASON::kNone);

					m_tokenRefr->SetDelete(true);
					m_tokenRefr->SetWantsDelete(true);
					m_tokenRefr->Disable();
				}

				m_tokenRefr.reset();
				m_frameRefr.reset();
				m_isActive = false;
			}

			return RE::BSEventNotifyControl::kContinue;
		}

	private:
		[[nodiscard]] static RE::ObjectRefHandle CreateWorkbench(RE::TESBoundObject* a_workbench)
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				auto data = RE::NEW_REFR_DATA();
				data.location = PlayerCharacter->data.location;
				data.direction = PlayerCharacter->data.angle;
				data.interior = PlayerCharacter->parentCell;
				data.world = data.interior ? data.interior->worldSpace : nullptr;
				data.object = a_workbench;

				if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
				{
					return TESDataHandler->CreateReferenceAtLocation(data);
				}
			}

			return RE::ObjectRefHandle();
		}

		RE::ObjectRefHandle m_workshop;
		RE::BSTSmartPointer<RE::TESObjectREFR, RE::BSTSmartPointerGamebryoRefCount> m_frameRefr;
		RE::BSTSmartPointer<RE::TESObjectREFR, RE::BSTSmartPointerGamebryoRefCount> m_tokenRefr;
		bool m_isActive{ false };
		bool m_hasSound{ false };
	};
}
*/
