#pragma once

#include "Config/Config.h"

namespace Patches
{
	class LoadEditorIDs
	{
	private:
		static void AddToGameMap(RE::TESForm* a_this, const char* a_edid)
		{
			const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
			const RE::BSAutoWriteLock locker{ lock.get() };
			if (map)
			{
				if (Config::Patches::bEnableEDIDConflictCheck)
				{
					auto iter = map->find(a_edid);
					if (iter != map->end())
					{
						const auto lhs = a_this->GetFormID();
						const auto rhs = iter->second->GetFormID();
						if (lhs != rhs)
						{
							REX::WARN("EditorID Conflict: {:08X} and {:08X} are both {}"sv, lhs, rhs, a_edid);
						}
						return;
					}
				}

				map->emplace(a_edid, a_this);
				rmap.emplace(a_this->formID, a_edid);
			}
		}

		inline static std::unordered_map<std::uint32_t, std::string> rmap;

	private:
		template <class T>
		class hkFormEditorID :
			public REX::Singleton<hkFormEditorID<T>>
		{
		private:
			static const char* GetFormEditorID(RE::TESForm* a_this)
			{
				auto it = rmap.find(a_this->formID);
				if (it != rmap.end())
				{
					return it->second.c_str();
				}

				return _GetFormEditorID(a_this);
			}

			static bool SetFormEditorID(RE::TESForm* a_this, const char* a_edid)
			{
				auto edid = std::string{ a_edid };
				if (a_this->formID < 0xFF000000 && !edid.empty())
				{
					AddToGameMap(a_this, a_edid);
				}

				return _SetFormEditorID(a_this, a_edid);
			}

			inline static REL::Relocation<decltype(&GetFormEditorID)> _GetFormEditorID;
			inline static REL::Relocation<decltype(&SetFormEditorID)> _SetFormEditorID;

		public:
			static void Install()
			{
				static REL::Relocation target{ T::VTABLE[0] };
				_GetFormEditorID = target.write_vfunc(0x3A, GetFormEditorID);
				_SetFormEditorID = target.write_vfunc(0x3B, SetFormEditorID);
			}
		};

	public:
		static void Install()
		{
			// hkFormEditorID<RE::BGSKeyword>::Install();
			// hkFormEditorID<RE::BGSLocationRefType>::Install();
			// hkFormEditorID<RE::BGSAction>::Install();
			hkFormEditorID<RE::BGSTransform>::Install();
			hkFormEditorID<RE::BGSComponent>::Install();
			hkFormEditorID<RE::BGSTextureSet>::Install();
			// hkFormEditorID<RE::BGSMenuIcon>::Install();
			// hkFormEditorID<RE::TESGlobal>::Install();
			hkFormEditorID<RE::BGSDamageType>::Install();
			hkFormEditorID<RE::TESClass>::Install();
			hkFormEditorID<RE::TESFaction>::Install();
			// hkFormEditorID<RE::BGSHeadPart>::Install();
			hkFormEditorID<RE::TESEyes>::Install();
			// hkFormEditorID<RE::TESRace>::Install();
			// hkFormEditorID<RE::TESSound>::Install();
			hkFormEditorID<RE::BGSAcousticSpace>::Install();
			hkFormEditorID<RE::EffectSetting>::Install();
			hkFormEditorID<RE::Script>::Install();
			hkFormEditorID<RE::TESLandTexture>::Install();
			hkFormEditorID<RE::EnchantmentItem>::Install();
			hkFormEditorID<RE::SpellItem>::Install();
			hkFormEditorID<RE::ScrollItem>::Install();
			hkFormEditorID<RE::TESObjectACTI>::Install();
			hkFormEditorID<RE::BGSTalkingActivator>::Install();
			hkFormEditorID<RE::TESObjectARMO>::Install();
			hkFormEditorID<RE::TESObjectBOOK>::Install();
			hkFormEditorID<RE::TESObjectCONT>::Install();
			hkFormEditorID<RE::TESObjectDOOR>::Install();
			hkFormEditorID<RE::IngredientItem>::Install();
			hkFormEditorID<RE::TESObjectLIGH>::Install();
			hkFormEditorID<RE::TESObjectMISC>::Install();
			hkFormEditorID<RE::TESObjectSTAT>::Install();
			// hkFormEditorID<RE::BGSStaticCollection>::Install();
			// hkFormEditorID<RE::BGSMovableStatic>::Install();
			hkFormEditorID<RE::TESGrass>::Install();
			hkFormEditorID<RE::TESObjectTREE>::Install();
			hkFormEditorID<RE::TESFlora>::Install();
			hkFormEditorID<RE::TESFurniture>::Install();
			hkFormEditorID<RE::TESObjectWEAP>::Install();
			hkFormEditorID<RE::TESAmmo>::Install();
			hkFormEditorID<RE::TESNPC>::Install();
			hkFormEditorID<RE::TESLevCharacter>::Install();
			hkFormEditorID<RE::TESKey>::Install();
			hkFormEditorID<RE::AlchemyItem>::Install();
			hkFormEditorID<RE::BGSIdleMarker>::Install();
			hkFormEditorID<RE::BGSNote>::Install();
			hkFormEditorID<RE::BGSProjectile>::Install();
			hkFormEditorID<RE::BGSHazard>::Install();
			hkFormEditorID<RE::BGSBendableSpline>::Install();
			hkFormEditorID<RE::TESSoulGem>::Install();
			hkFormEditorID<RE::BGSTerminal>::Install();
			hkFormEditorID<RE::TESLevItem>::Install();
			hkFormEditorID<RE::TESWeather>::Install();
			hkFormEditorID<RE::TESClimate>::Install();
			hkFormEditorID<RE::BGSShaderParticleGeometryData>::Install();
			hkFormEditorID<RE::BGSReferenceEffect>::Install();
			hkFormEditorID<RE::TESRegion>::Install();
			// hkFormEditorID<RE::NavMeshInfoMap>::Install();
			// hkFormEditorID<RE::TESObjectCELL>::Install();
			// hkFormEditorID<RE::TESObjectREFR>::Install();
			hkFormEditorID<RE::Explosion>::Install();
			hkFormEditorID<RE::Projectile>::Install();
			hkFormEditorID<RE::Actor>::Install();
			hkFormEditorID<RE::PlayerCharacter>::Install();
			hkFormEditorID<RE::MissileProjectile>::Install();
			hkFormEditorID<RE::ArrowProjectile>::Install();
			hkFormEditorID<RE::GrenadeProjectile>::Install();
			hkFormEditorID<RE::BeamProjectile>::Install();
			hkFormEditorID<RE::FlameProjectile>::Install();
			hkFormEditorID<RE::ConeProjectile>::Install();
			hkFormEditorID<RE::BarrierProjectile>::Install();
			hkFormEditorID<RE::Hazard>::Install();
			// hkFormEditorID<RE::TESWorldSpace>::Install();
			// hkFormEditorID<RE::TESObjectLAND>::Install();
			// hkFormEditorID<RE::NavMesh>::Install();
			// hkFormEditorID<RE::TESTopic>::Install();
			hkFormEditorID<RE::TESTopicInfo>::Install();
			// hkFormEditorID<RE::TESQuest>::Install();
			// hkFormEditorID<RE::TESIdleForm>::Install();
			hkFormEditorID<RE::TESPackage>::Install();
			hkFormEditorID<RE::AlarmPackage>::Install();
			hkFormEditorID<RE::DialoguePackage>::Install();
			hkFormEditorID<RE::FleePackage>::Install();
			hkFormEditorID<RE::SpectatorPackage>::Install();
			hkFormEditorID<RE::TrespassPackage>::Install();
			hkFormEditorID<RE::TESCombatStyle>::Install();
			hkFormEditorID<RE::TESLoadScreen>::Install();
			hkFormEditorID<RE::TESLevSpell>::Install();
			// hkFormEditorID<RE::TESObjectANIO>::Install();
			hkFormEditorID<RE::TESWaterForm>::Install();
			hkFormEditorID<RE::TESEffectShader>::Install();
			hkFormEditorID<RE::BGSExplosion>::Install();
			hkFormEditorID<RE::BGSDebris>::Install();
			hkFormEditorID<RE::TESImageSpace>::Install();
			// hkFormEditorID<RE::TESImageSpaceModifier>::Install();
			hkFormEditorID<RE::BGSListForm>::Install();
			hkFormEditorID<RE::BGSPerk>::Install();
			hkFormEditorID<RE::BGSBodyPartData>::Install();
			hkFormEditorID<RE::BGSAddonNode>::Install();
			// hkFormEditorID<RE::ActorValueInfo>::Install();
			hkFormEditorID<RE::BGSCameraShot>::Install();
			hkFormEditorID<RE::BGSCameraPath>::Install();
			// hkFormEditorID<RE::BGSVoiceType>::Install();
			hkFormEditorID<RE::BGSMaterialType>::Install();
			hkFormEditorID<RE::BGSImpactData>::Install();
			hkFormEditorID<RE::BGSImpactDataSet>::Install();
			hkFormEditorID<RE::TESObjectARMA>::Install();
			hkFormEditorID<RE::BGSEncounterZone>::Install();
			hkFormEditorID<RE::BGSLocation>::Install();
			hkFormEditorID<RE::BGSMessage>::Install();
			// hkFormEditorID<RE::BGSDefaultObjectManager>::Install();
			// hkFormEditorID<RE::BGSDefaultObject>::Install();
			hkFormEditorID<RE::BGSLightingTemplate>::Install();
			// hkFormEditorID<RE::BGSMusicType>::Install();
			hkFormEditorID<RE::BGSFootstep>::Install();
			hkFormEditorID<RE::BGSFootstepSet>::Install();
			// hkFormEditorID<RE::BGSStoryManagerBranchNode>::Install();
			// hkFormEditorID<RE::BGSStoryManagerQuestNode>::Install();
			// hkFormEditorID<RE::BGSStoryManagerEventNode>::Install();
			hkFormEditorID<RE::BGSDialogueBranch>::Install();
			hkFormEditorID<RE::BGSMusicTrackFormWrapper>::Install();
			hkFormEditorID<RE::TESWordOfPower>::Install();
			hkFormEditorID<RE::TESShout>::Install();
			hkFormEditorID<RE::BGSEquipSlot>::Install();
			hkFormEditorID<RE::BGSRelationship>::Install();
			hkFormEditorID<RE::BGSScene>::Install();
			hkFormEditorID<RE::BGSAssociationType>::Install();
			hkFormEditorID<RE::BGSOutfit>::Install();
			hkFormEditorID<RE::BGSArtObject>::Install();
			hkFormEditorID<RE::BGSMaterialObject>::Install();
			hkFormEditorID<RE::BGSMovementType>::Install();
			// hkFormEditorID<RE::BGSSoundDescriptorForm>::Install();
			hkFormEditorID<RE::BGSDualCastData>::Install();
			hkFormEditorID<RE::BGSSoundCategory>::Install();
			hkFormEditorID<RE::BGSSoundOutput>::Install();
			hkFormEditorID<RE::BGSCollisionLayer>::Install();
			hkFormEditorID<RE::BGSColorForm>::Install();
			hkFormEditorID<RE::BGSReverbParameters>::Install();
			// hkFormEditorID<RE::BGSPackIn>::Install();
			hkFormEditorID<RE::BGSAimModel>::Install();
			hkFormEditorID<RE::BGSConstructibleObject>::Install();
			hkFormEditorID<RE::BGSMod::Attachment::Mod>::Install();
			hkFormEditorID<RE::BGSMaterialSwap>::Install();
			hkFormEditorID<RE::BGSZoomData>::Install();
			hkFormEditorID<RE::BGSInstanceNamingRules>::Install();
			hkFormEditorID<RE::BGSSoundKeywordMapping>::Install();
			hkFormEditorID<RE::BGSAudioEffectChain>::Install();
			// hkFormEditorID<RE::BGSAttractionRule>::Install();
			hkFormEditorID<RE::BGSSoundCategorySnapshot>::Install();
			hkFormEditorID<RE::BGSSoundTagSet>::Install();
			hkFormEditorID<RE::BGSLensFlare>::Install();
			hkFormEditorID<RE::BGSGodRays>::Install();

			REX::DEBUG("Installed Patch: LoadEditorIDs"sv);
		}
	};
}
