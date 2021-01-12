#pragma once

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/chrono.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "AutoTOML.hpp"

#define DLLEXPORT __declspec(dllexport)

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

namespace logger
{
	using namespace F4SE::log;
}

namespace stl
{
	using F4SE::stl::report_and_fail;
}

using namespace std::literals;

#include "version.h"

#include "Settings.h"