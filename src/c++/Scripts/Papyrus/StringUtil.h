#pragma once

namespace Papyrus
{
	namespace StringUtil
	{
		constexpr auto SCRIPT_NAME = "StringUtil"sv;

		std::int32_t GetLength(std::monostate, std::string_view a_string)
		{
			return static_cast<std::int32_t>(a_string.length());
		}

		std::string GetNthCharacter(std::monostate, std::string_view a_string, std::int32_t a_idx)
		{
			std::string result{ "" };
			if (a_idx >= 0 && a_string.length() > a_idx)
			{
				result = a_string[a_idx];
			}

			return result;
		}

		std::string AsChar(std::monostate, std::int32_t a_charCode)
		{
			std::string result{ static_cast<char>(a_charCode) };
			return result;
		}

		bool IsLetter(std::monostate, std::string_view a_string)
		{
			return a_string.empty() ? false : std::isalpha(a_string[0]);
		}

		bool IsDigit(std::monostate, std::string_view a_string)
		{
			return a_string.empty() ? false : std::isdigit(a_string[0]);
		}

		bool IsPunctuation(std::monostate, std::string_view a_string)
		{
			return a_string.empty() ? false : std::ispunct(a_string[0]);
		}

		bool IsPrintable(std::monostate, std::string_view a_string)
		{
			return a_string.empty() ? false : std::isprint(a_string[0]);
		}

		std::int32_t AsInt(std::monostate, std::string_view a_string)
		{
			return a_string.empty() ? -1 : static_cast<std::int32_t>(a_string[0]);
		}

		std::int32_t Find(std::monostate, std::string_view a_string, std::string_view a_subStr, std::int32_t a_idx)
		{
			auto result = a_string.find(a_subStr, a_idx);
			return static_cast<std::int32_t>(result);
		}

		std::string Substring(std::monostate, std::string_view a_string, std::int32_t a_idx, std::int32_t a_length)
		{
			std::string result{ "" };
			if (a_string.length() >= a_idx)
			{
				result = a_string.substr(a_idx, a_length);
			}

			return result;
		}

		std::vector<std::string> StringToStringArray(std::monostate, std::string_view a_string, std::string_view a_delim)
		{
			std::vector<std::string> result;

			if (a_delim.empty())
			{
				for (auto i = 0; i < a_string.length(); i++)
				{
					std::string element{ a_string[i] };
					result.emplace_back(element);
				}
			}
			else
			{
				std::size_t i = 0;
				do
				{
					auto bIdx = a_string.find_first_not_of(a_delim, i);
					auto eIdx = a_string.find_first_of(a_delim, bIdx);

					if (bIdx >= 0)
					{
						std::string element{ a_string.substr(bIdx, eIdx - bIdx) };
						result.emplace_back(element);
						i = eIdx;
					}
					else
					{
						std::string element{ a_string.substr(i) };
						result.emplace_back(element);
						i = a_string.length();
					}
				}
				while (i < a_string.length());
			}

			return result;
		}

		std::string StringArrayToString(std::monostate, std::vector<std::string_view> a_array, std::string_view a_delim)
		{
			std::string result{ "" };
			for (auto iter : a_array)
			{
				result.append(iter);
				if (!a_delim.empty())
				{
					result.append(a_delim);
				}
			}

			return result;
		}
	}
}
