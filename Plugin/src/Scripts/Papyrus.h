#pragma once

#include "Papyrus/BakaUtil.h"
#include "Papyrus/StringUtil.h"

namespace Papyrus
{
    bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
    {
        // BakaUtil
        a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "FilterReferencesByKeywords", BakaUtil::FilterReferencesByKeywords, true);
        a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "OpenWebPage", BakaUtil::OpenWebPage, true);
        a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "RegisterForPipboyLightEvent", BakaUtil::RegisterForPipboyLightEvent, true);
        a_VM->BindNativeMethod(BakaUtil::SCRIPT_NAME, "UnregisterForPipboyLightEvent", BakaUtil::UnregisterForPipboyLightEvent, true);
        RE::PipboyLightEvent::GetEventSource()->RegisterSink(BakaUtil::detail::PipboyLightEventHandler::GetSingleton());

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
