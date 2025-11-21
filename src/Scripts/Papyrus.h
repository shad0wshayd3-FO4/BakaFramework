#pragma once

#include "Papyrus/BakaUtil.h"
#include "Papyrus/StringUtil.h"

namespace Papyrus
{
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
	{
		// BakaUtil
		a_VM->BindNativeMethod("BakaUtil"sv, "FilterReferencesByKeywords"sv, BakaUtil::FilterReferencesByKeywords, true);
		a_VM->BindNativeMethod("BakaUtil"sv, "RegisterForPipboyLightEvent"sv, BakaUtil::RegisterForPipboyLightEvent, true);
		a_VM->BindNativeMethod("BakaUtil"sv, "UnregisterForPipboyLightEvent"sv, BakaUtil::UnregisterForPipboyLightEvent, true);
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(BakaUtil::PipboyLightEventHandler::GetSingleton());

		// StringUtil
		a_VM->BindNativeMethod("StringUtil"sv, "GetLength"sv, StringUtil::GetLength, true);
		a_VM->BindNativeMethod("StringUtil"sv, "GetNthCharacter"sv, StringUtil::GetNthCharacter, true);
		a_VM->BindNativeMethod("StringUtil"sv, "IsLetter"sv, StringUtil::IsLetter, true);
		a_VM->BindNativeMethod("StringUtil"sv, "IsDigit"sv, StringUtil::IsDigit, true);
		a_VM->BindNativeMethod("StringUtil"sv, "IsPunctuation"sv, StringUtil::IsPunctuation, true);
		a_VM->BindNativeMethod("StringUtil"sv, "IsPrintable"sv, StringUtil::IsPrintable, true);
		a_VM->BindNativeMethod("StringUtil"sv, "AsInt"sv, StringUtil::AsInt, true);
		a_VM->BindNativeMethod("StringUtil"sv, "AsChar"sv, StringUtil::AsChar, true);
		a_VM->BindNativeMethod("StringUtil"sv, "Find"sv, StringUtil::Find, true);
		a_VM->BindNativeMethod("StringUtil"sv, "Substring"sv, StringUtil::Substring, true);
		a_VM->BindNativeMethod("StringUtil"sv, "StringToArray"sv, StringUtil::StringToStringArray, true);
		a_VM->BindNativeMethod("StringUtil"sv, "ArrayToString"sv, StringUtil::StringArrayToString, true);

		return true;
	}
}
