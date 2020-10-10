#pragma once

namespace RE
{
	namespace BSScript
	{
		struct StaticFunctionTag;
	}
}

namespace Papyrus
{
	namespace StringUtil
	{
		std::uint32_t GetLength(RE::BSScript::StaticFunctionTag*, RE::BSFixedString a_string)
		{
			return a_string.length();
		}

		// BSFixedString GetNthCharacter(string, idx)
		// BSFixedString AsChar(idx)
		// bool IsLetter(string)
		// bool IsDigit(string)
		// bool IsPunctuation(string)
		// bool IsPrintable(string)
		// std::uint32_t AsInt(string)
		// std::uint32_t Find(string, string, idx)
		// BSFixedString SubStr(string, idx, len)
		// BSFixedString[] StringToStringArray(string, delim)
		// BSFixedString StringArrayToString(string[], delim)
	}
}