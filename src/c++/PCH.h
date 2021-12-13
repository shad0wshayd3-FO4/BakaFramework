#pragma once

#pragma warning(push, 0)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <AutoTOML.hpp>
#include <fmt/chrono.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace logger = F4SE::log;

namespace stl
{
	using F4SE::stl::emplace_vtable;
	using F4SE::stl::report_and_fail;
	using F4SE::stl::to_underlying;

#ifdef F4SE_SUPPORT_XBYAK
	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);
#endif
}

namespace F4SE
{
	namespace WinAPI
	{
		inline constexpr auto(MAX_COMPUTERNAME_LENGTH){ static_cast<std::uint32_t>(0x0F) };

		[[nodiscard]] bool(GetComputerName)(
			char* a_computerName,
			std::uint32_t* a_size) noexcept;
	}
}

#include "Plugin.h"

#include "Settings.h"

#define DLLEXPORT __declspec(dllexport)
