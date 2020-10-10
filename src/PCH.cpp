#include "PCH.h"

#include <Windows.h>

#undef GetComputerName

namespace F4SE
{
	namespace WinAPI
	{
		bool(GetComputerName)(
			char* a_computerName,
			std::uint32_t* a_size) noexcept
		{
			return static_cast<bool>(
				::GetComputerNameA(
					static_cast<::LPSTR>(a_computerName),
					reinterpret_cast<::LPDWORD>(a_size)));
		}
	}
}