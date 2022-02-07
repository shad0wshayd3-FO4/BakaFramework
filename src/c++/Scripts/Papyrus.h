#pragma once

#include "Papyrus/BakaUtil.h"
#include "Papyrus/ScriptObject.h"
#include "Papyrus/StringUtil.h"

namespace Papyrus
{
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
	{
		// BakaUtil
		a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "CreatePowerArmorToken", BakaUtil::CreatePowerArmorToken, true);
		a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "HandlePowerArmorToken", BakaUtil::HandlePowerArmorToken, true);
		a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "FilterReferencesByKeywords", BakaUtil::FilterReferencesByKeywords, true);
		a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "OpenWebPage", BakaUtil::OpenWebPage, true);

		// ScriptObject
		a_VM->BindNativeMethod(ScriptObject::SCRIPT_NAME, "RegisterForPipboyLightEvent", ScriptObject::RegisterForPipboyLightEvent, true);
		a_VM->BindNativeMethod(ScriptObject::SCRIPT_NAME, "UnregisterForPipboyLightEvent", ScriptObject::UnregisterForPipboyLightEvent, true);
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(ScriptObject::PipboyLightEventHandler::GetSingleton());

		// StringUtil
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "GetLength", StringUtil::GetLength, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "GetNthCharacter", StringUtil::GetNthCharacter, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "IsLetter", StringUtil::IsLetter, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "IsDigit", StringUtil::IsDigit, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "IsPunctuation", StringUtil::IsPunctuation, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "IsPrintable", StringUtil::IsPrintable, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "AsInt", StringUtil::AsInt, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "AsChar", StringUtil::AsChar, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "Find", StringUtil::Find, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "Substring", StringUtil::Substring, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "StringToArray", StringUtil::StringToStringArray, true);
		a_VM->BindNativeMethod(StringUtil::SCRIPT_NAME, "ArrayToString", StringUtil::StringArrayToString, true);

		return true;
	}
}
