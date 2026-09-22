#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/GetBaseObject.h"
#include "ObScript/GetDefaultObject.h"
#include "ObScript/Help.h"
#include "ObScript/SpellHasCastingPerk.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::GetBaseObject::Install();
		ObScript::GetDefaultObject::Install();
		ObScript::Help::Install();
		ObScript::SpellHasCastingPerk::Install();
	}
}

#include "Papyrus/BakaEvents.h"
#include "Papyrus/BakaUtil.h"
#include "Papyrus/StringUtil.h"

namespace Papyrus
{
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm)
	{
		// BakaEvents
		BakaEvents::RegisterFunctions(a_vm);

		// BakaUtil
		a_vm->BindNativeMethod("BakaUtil"sv, "FilterReferencesByKeywords"sv, BakaUtil::FilterReferencesByKeywords, true);

		// StringUtil
		a_vm->BindNativeMethod("StringUtil"sv, "GetLength"sv, StringUtil::GetLength, true);
		a_vm->BindNativeMethod("StringUtil"sv, "GetNthCharacter"sv, StringUtil::GetNthCharacter, true);
		a_vm->BindNativeMethod("StringUtil"sv, "IsLetter"sv, StringUtil::IsLetter, true);
		a_vm->BindNativeMethod("StringUtil"sv, "IsDigit"sv, StringUtil::IsDigit, true);
		a_vm->BindNativeMethod("StringUtil"sv, "IsPunctuation"sv, StringUtil::IsPunctuation, true);
		a_vm->BindNativeMethod("StringUtil"sv, "IsPrintable"sv, StringUtil::IsPrintable, true);
		a_vm->BindNativeMethod("StringUtil"sv, "AsInt"sv, StringUtil::AsInt, true);
		a_vm->BindNativeMethod("StringUtil"sv, "AsChar"sv, StringUtil::AsChar, true);
		a_vm->BindNativeMethod("StringUtil"sv, "Find"sv, StringUtil::Find, true);
		a_vm->BindNativeMethod("StringUtil"sv, "Substring"sv, StringUtil::Substring, true);
		a_vm->BindNativeMethod("StringUtil"sv, "StringToArray"sv, StringUtil::StringToStringArray, true);
		a_vm->BindNativeMethod("StringUtil"sv, "ArrayToString"sv, StringUtil::StringArrayToString, true);

		return true;
	}
}
