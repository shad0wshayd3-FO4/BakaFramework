#pragma once

#include "Papyrus/BakaUtil.h"
#include "Papyrus/StringUtil.h"

namespace Papyrus
{
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
	{
		// BakaUtil
		a_VM->BindNativeMethod("BakaUtil", "FilterReferencesByKeywords", BakaUtil::FilterReferencesByKeywords, true);

		/*
		a_VM->BindNativeMethod("ScriptObject", "RegisterForPipboyLightEvent",
			BakaUtil::RegisterForPipboyLightEvent);

		a_VM->BindNativeMethod("ScriptObject", "UnregisterForPipboyLightEvent",
			BakaUtil::UnregisterForPipboyLightEvent);
		*/

		// StringUtil
		a_VM->BindNativeMethod("StringUtil", "GetLength", StringUtil::GetLength, true);
		a_VM->BindNativeMethod("StringUtil", "GetNthCharacter", StringUtil::GetNthCharacter, true);
		a_VM->BindNativeMethod("StringUtil", "IsLetter", StringUtil::IsLetter, true);
		a_VM->BindNativeMethod("StringUtil", "IsDigit", StringUtil::IsDigit, true);
		a_VM->BindNativeMethod("StringUtil", "IsPunctuation", StringUtil::IsPunctuation, true);
		a_VM->BindNativeMethod("StringUtil", "IsPrintable", StringUtil::IsPrintable, true);
		a_VM->BindNativeMethod("StringUtil", "AsInt", StringUtil::AsInt, true);
		a_VM->BindNativeMethod("StringUtil", "AsChar", StringUtil::AsChar, true);
		a_VM->BindNativeMethod("StringUtil", "Find", StringUtil::Find, true);
		a_VM->BindNativeMethod("StringUtil", "Substring", StringUtil::Substring, true);
		a_VM->BindNativeMethod("StringUtil", "StringToArray", StringUtil::StringToStringArray, true);
		a_VM->BindNativeMethod("StringUtil", "ArrayToString", StringUtil::StringArrayToString, true);

		return true;
	}
}
